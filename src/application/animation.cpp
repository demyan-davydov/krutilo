// -*- coding:utf-8-unix -*-
/*!
    @file       animation.cpp
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-30

    @brief      Animation & timers support
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#include "animation.h"
#include <algorithm>
#include <limits>


Animation::Animation()
{

}

Animation::~Animation()
{

}

int Animation::add(void* owner, Timestamp startTime, Timestamp duration, Timestamp rest, int count)
{
    auto aId = getNextId();
    m_items.emplace(std::make_pair(Key(owner, aId), Item(startTime, duration, rest, count)));
    //m_items[Key(owner, getNextId())] = Item(startTime, duration, count);

    return aId;
}

int Animation::reset(void* owner, int animId, Timestamp startTime, Timestamp duration, Timestamp rest, int count)
{
    auto it = m_items.find(Key(owner, animId));

    if (it != m_items.end()) {
        it->second = Item(startTime, duration, rest, count);
        return it->first.animId;
    }
    else {
        return add(owner, startTime, duration, rest, count);
    }

    return 0;
}

bool Animation::remove(void* owner, int animId)
{
    auto erased1 = m_items.erase(Key(owner, animId));
    auto erased2 = m_hold_items.erase(Key(owner, animId));
    return (erased1 + erased2) != 0;
}

void Animation::removeByOwner(void* owner)
{
    removeByOwner(m_items, owner);
    removeByOwner(m_hold_items, owner);
}

void Animation::clear()
{
    m_items.clear();
}

bool Animation::hold(void* owner, int animId)
{
    return moveItem(m_items, m_hold_items, owner, animId);
}

bool Animation::restore(void* owner, int animId)
{
    return moveItem(m_hold_items, m_items, owner, animId);
}

void Animation::enumerate(Timestamp currTime, std::function<void(void* owner, int animId, float progress, int count)>&& fn)
{
    for (auto i = m_items.begin(); i != m_items.end(); ++i)
    {
        auto const& k = i->first;   // key
        auto& v = i->second;        // value

        if (currTime > v.start) {
            const auto least = (currTime - v.start) % v.period();
            auto count = (currTime - v.start) / v.period();
            
            auto least2 = least;
            if (least2 > v.duration) {
                least2 = v.duration;
            }

            if (v.countMax > 0 && count >= (unsigned)v.countMax) { // snap last update to 100%
                count = (unsigned)(v.countMax - 1);
                least2 = v.duration;
            }

            const auto progress = v.duration ? (float)least2 / v.duration : 1.0f;
            const auto updateTime = v.start + count * v.period() + least2;

            if (updateTime != v.lastupd) // exactly this animation step not yet processed
                fn(k.owner, k.animId, progress, count);

            v.lastupd = updateTime;
        }
    }
}

void Animation::removeEstimined(Timestamp currTime)
{
    removeEstiminedBase(m_items, currTime);
    removeEstiminedBase(m_hold_items, currTime);
}

/// remove estimined animations, before enumerate removed animations
void Animation::removeEstimined(Timestamp currTime, std::function<void(void* owner, int animId)> const& fn)
{
    removeEstiminedBase(m_items, currTime, fn);
    removeEstiminedBase(m_hold_items, currTime, fn);
}

int Animation::getNextId()
{
    return ++m_lastId;
}

void Animation::freeID(int animationId)
{
    // do nothing yet
    // todo: allocate and free unused id's
}

void Animation::freeID(iter i)
{
    freeID(i->first.animId);
}

void Animation::freeIDs(iter_range r)
{
    for (auto i = r.first; i != r.second; ++i) {
        freeID(i);
    }
}

Animation::iter Animation::firstByOwner(container& items, void* owner)
{
    void* prev = (void*)(((char*)owner) - 1);
    auto it = items.upper_bound( Key(prev, std::numeric_limits<int>::max()) );
    return it;
}

Animation::iter_range Animation::rangeByOwner(container& items, void* owner)
{
    auto a = firstByOwner(items, owner);
    auto b = a;
    if (b != items.end()) ++b;

    while (b != items.end()) {
        if (b->first.owner != owner) 
            break;
        ++b;
    }

    return std::make_pair(a, b);
}

void Animation::removeByOwner(container& items, void* owner) 
{
    auto r = rangeByOwner(items, owner);
    freeIDs(r);
    items.erase(r.first, r.second);
}

bool Animation::moveItem(container& from, container& to, void* owner, int animId) // static
{
    auto ifrom = from.find(Key(owner, animId));
    if (ifrom != from.end()) {
        to.emplace(*ifrom);
        from.erase(ifrom);
        return true;
    }
    return false;
}

void Animation::removeEstiminedBase(container& from, Timestamp currTime) // static
{
    for (auto i = from.begin(); i != from.end();)
    {
        auto const& v = i->second;
        auto nxt = i; ++nxt;

        if (!v.isInfinite()) {
            auto endTime = v.endTime();
            if (currTime > endTime) {
                auto nxt = i; ++nxt;
                freeID(i);
                from.erase(i);    
                i = nxt;
            }
        }
        i = nxt;
    }
}

void Animation::removeEstiminedBase(container& from, Timestamp currTime, std::function<void(void* owner, int animId)> const& fn)
{
    for (auto i = from.begin(); i != from.end();)
    {
        auto const& v = i->second;
        auto nxt = i; ++nxt;

        if (!v.isInfinite()) {
            auto endTime = v.endTime();
            if (currTime > endTime) {
                //auto nxt = i; ++nxt;
                fn(i->first.owner, i->first.animId);
                freeID(i);
                from.erase(i);    
                //i = nxt;
            }
        }
        i = nxt;
    }
}
