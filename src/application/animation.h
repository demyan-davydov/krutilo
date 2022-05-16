// -*- coding:utf-8-unix -*-
/*!
    @file       animation.h
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

#pragma once
#ifndef file__ANIMATION_H
#define file__ANIMATION_H

#include "mbed.h" // timestamp_t
#include <map>
#include <functional>

class Animation
{
    typedef timestamp_t Timestamp;

    /// Animation item
    struct Item
    {
        Item(Timestamp startTimeMs, Timestamp durationMs, Timestamp restMs, int maxCount) 
        : start(startTimeMs)
        , duration(durationMs)
        , rest(restMs)
        , lastupd(0)
        , count(0)
        , countMax(maxCount)
        {
        }

        Timestamp start;        ///< absolute start time 
        Timestamp duration;     ///< animation duration (ms)
        Timestamp rest;         ///< pause between animations (ms)
        Timestamp lastupd;      ///< absolute time of last updation (enumeration)
        int count;              ///< current count of animation 
        int countMax;           ///< maximal count of animation periods

        Timestamp endTime() const { return start + period() * countMax; }

        /// animation period
        Timestamp period() const { return duration + rest; }

        /// is infinite animation
        bool isInfinite() const { return countMax == -1; }

        Item& operator = (Item const& o) = default;
    };

    /// todo: maybe keep owner and animId inside one variable?
    struct Key
    {
        Key(void* o, int aId) : owner((char*)o), animId(aId) {}
        char* owner;
        int animId;

        /// first compare 'owner' fields, if they are equal, comapre 'animId'
        bool operator < (Key const& o) const 
        {
            return (owner == o.owner) ? animId < o.animId : owner < o.owner;
        }
    };

    typedef std::map<Key, Item> container;
    container m_items;
    container m_hold_items;
    int m_lastId = 0;
    static constexpr int k_invalidId = 0;

    typedef container::iterator iter;
    typedef std::pair<iter, iter> iter_range;

public:
    Animation();
    ~Animation();

    /// add new animation
    int add(void* owner, Timestamp startTime, Timestamp duration, Timestamp rest, int count);
    
    /// reset specified animation
    /// if animation exists, reset it's parameters, 
    /// if not -- create new animation
    int reset(void* owner, int animId, Timestamp startTime, Timestamp duration, Timestamp rest, int count);
    
    /// remove specified animation
    bool remove(void* owner, int animId);
    
    /// remove all animations by OwnerId
    void removeByOwner(void* owner);
    
    /// remove all animations
    void clear();

    /// temporary hold animation 
    bool hold(void* owner, int animId);

    /// restore holded animation
    bool restore(void* owner, int animId);

    /// enumerate existing non holded animations
    void enumerate(Timestamp currTime, std::function<void(void* owner, int animId, float progress, int count)>&& fn);
    
    /// remove estimined animations
    void removeEstimined(Timestamp currTime);

    /// remove estimined animations, before enumerate removed animations
    void removeEstimined(Timestamp currTime, std::function<void(void* owner, int animId)> const& fn);

private:
    int getNextId(); ///< generate new animation id and return it
    void freeID(int animationId);
    void freeID(iter i);
    void freeIDs(iter_range);
    static iter firstByOwner(container& c, void* owner);
    static iter_range rangeByOwner(container& c, void* owner);
    void removeByOwner(container& c, void* owner);
    static bool moveItem(container& from, container& to, void* owner, int animId);
    void removeEstiminedBase(container& from, Timestamp t);
    void removeEstiminedBase(container& from, Timestamp t, std::function<void(void* owner, int animId)> const& fn);
};

#endif // file__ANIMATION_H