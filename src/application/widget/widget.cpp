// -*- coding:utf-8-unix -*-
/*!
    @file       widget.cpp
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Base Widget class
    @description
        Any widget bust be derived from Widget

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#include "widget.h"
#include "ui_style.h"
#include "../application.h"


Widget::Widget(Widget* parent, GCRect const& rc, const State& initState)
: m_parent(parent)
, m_state(initState)
, m_rect(rc)
{
    anchor(Anchor::TopLeft);
    app().postMessage(this, MsgInit{});
}

Widget::Widget(Widget* parent, const State& initState)
: Widget(parent, GCRect::createEmpty(0,0), initState)
{

}

Widget::Widget(Widget* parent, GCPoint const& pt, GCSize const& sz, Anchor a, const State& initState)
: Widget(parent, createRelative(pt, sz, a), initState)
{

}

/*Widget* Widget::create(Widget* parent, const State& initState = State{})
{
    return new Widget(parent, initState);
}

Widget* Widget::create(Widget* parent, GCRect const& rc, const State& initState = State{})
{
    return new Widget(parent, rc, initState);
}

Widget* Widget::create(Widget* parent, GCPoint const& pt, GCSize const& sz, Anchor a, const State& initState = State{})
{
    return new Widget(parent, pt, sz, a, initState);
}*/

Widget::~Widget()
{
    app().widgetRemoved(this);
}

/*int Widget::uistate() const
{
    return (int)UIState::Normal;
}*/

uint16_t Widget::deep() const
{
    uint16_t res = 0;
    auto p = this;
    do {
        ++res;
    } while ((p = p->parent()));
    return res;
}

GCRect const& Widget::rect() const
{
    return m_rect;
}

GCSize Widget::size() const
{
    return m_rect.size();
}

GCRect Widget::rectLocal() const
{
    return GCRect::createSized({0,0}, m_rect.size());
}

GCPoint Widget::rootOffset() const
{
    GCPoint pt = {0,0}; //rect().topLeft();

    auto p = this;
    while(p) {
        pt.moveBy(p->rect().x1(), p->rect().y1());
        p = p->parent();
    }
    return pt;
}

void Widget::setFocus(bool focus)
{
    if (hasFocus() != focus )
    if (tabStop()) {
        // repaint prev

        if (focus) {
            if (reallyVisible())
            {
                Widget* prev = app().getFocusWidget();

                app().setFocusWidget(this);
                onFocus(true);
                sendRepaint();

                if (prev) {
                    prev->onFocus(false);

                    if (prev->reallyVisible())
                        prev->sendRepaint();
                }
            }
        }
        else { // 
            app().setFocusWidget(nullptr);
            onFocus(false);
            sendRepaint();
        }
    }
}

bool Widget::hasFocus() const 
{
    return app().getFocusWidget() == this;
    //return state_.hasFocus; 
}

bool Widget::isChecked() const
{
    return m_state.checked();
}

void Widget::setChecked(bool v)
{
    const auto ca = checkable();

    if (ca == Checkable::Checkbox) {
        m_state.checked(v);
        sendRepaint();
    }
    else if (ca == Checkable::Radiobutton) {
        /// clear check state for any other 'radio' siblings
        if (parent()) {
            for (auto& c : parent()->m_childs) {
                if (Checkable::Radiobutton == c.checkable()) {
                    c.m_state.checked(false);
                }
            }
        }

        m_state.checked(v);
        sendRepaint();
    }
}

void Widget::toggleChecked()
{
    setChecked(!isChecked());
}

void Widget::setVisible(bool visible, bool repaintParent)
{
    if (m_state.visible() != visible) {
        m_state.visible(visible);
        notifyShow(visible);

        if (visible) {
            sendRepaint();
        }
        else if (parent() && repaintParent) {
            parent()->sendRepaint(rect());
        }

        //notifyShow(visible);
    }
}

bool Widget::isVisible() const
{
    return m_state.visible();
}

bool Widget::reallyVisible() const
{
    return isVisible() && (parent() ? parent()->reallyVisible() : true);
}

void Widget::setEnabled(bool enable)
{
    if (m_state.enabled() != enable) {
        m_state.enabled(enable);
        sendRepaint();
    }
}

bool Widget::isEnabled() const
{
    return m_state.enabled();
}

bool Widget::isInited() const
{
    return m_state.inited();
}

bool Widget::isRepaintBlocked() const
{
    return m_state.noRepaint();
}

void Widget::noRepaint(bool v)
{
    m_state.noRepaint(v);
}

Anchor Widget::anchor() const
{
    return m_state.anchor();
}

void Widget::anchor(Anchor a)
{
    m_state.anchor(a);
}

bool Widget::reallyEnabled() const
{
    return isEnabled() && (parent() ? parent()->reallyEnabled() : true);
}

void Widget::setRect(GCRect const& newRc)
{
    if (m_rect != newRc) {
        m_rect = newRc;
        // todo: repaint
    }
}

void Widget::setSize(GCSize const& newSz)
{
    setRect(resize(m_rect, newSz, anchor()));
}

uint32_t Widget::childCount() const
{
    return (uint32_t)m_childs.size();
}

Widget* Widget::nthChild(uint32_t n)
{
    if (n < m_childs.size()) {
        auto i = m_childs.begin();
        std::advance(i, n);
        return &*i;
    }
    return nullptr;
}

Widget* Widget::root()
{
    auto r = this;
    for (; r->parent(); r = r->parent() ) {}
    return r;
}

Widget* Widget::firstParent(std::function<bool(Widget*)> const& fn)
{
    auto p = parent();
    while (p && !fn(p)) {
        p = p->parent();
    }
    return p;
}

Widget* Widget::prevAncle()
{
    return parent() ? parent()->prevSibling() : nullptr; 
}

Widget* Widget::nextAncle()
{
    return parent() ? parent()->nextSibling() : nullptr; 
}

Widget* Widget::firstNextAncle()
{
    auto p = parent();
    while (p) {
        auto a = p->nextSibling();
        if (a) return a;
        p = p->parent();
    }
    return nullptr;
}

Widget* Widget::firstNextAncle(std::function<bool(Widget*)> const& fn)
{
    auto p = parent();
    while (p) {
        auto a = p->nextSibling(fn);
        if (a) return a;
        p = p->parent();
    }
    return nullptr;
}

Widget* Widget::firstPrevAncle()
{
    auto p = parent();
    while (p) {
        auto a = p->prevSibling();
        if (a) return a;
        p = p->parent();
    }
    return nullptr;
}

Widget* Widget::firstPrevAncle(std::function<bool(Widget*)> const& fn)
{
    auto p = parent();
    while (p) {
        auto a = p->prevSibling(fn);
        if (a) return a;
        p = p->parent();
    }
    return nullptr;
}

Widget* Widget::firstChild()
{
    return m_childs.empty() ? nullptr : &m_childs.front();
}

Widget* Widget::firstChild(std::function<bool(Widget*)> const& fn)
{
    if (!m_childs.empty()) {
        for (auto i = m_childs.begin(); i != m_childs.end(); ++i) {
            auto* ptr = &*i;
            if (fn(ptr)) {
                return ptr;
            }
        }
    }

    return nullptr;
}

Widget* Widget::lastChild()
{
    return m_childs.empty() ? nullptr : &m_childs.back();
}

Widget* Widget::lastChild(std::function<bool(Widget*)> const& fn)
{
    if (!m_childs.empty()) {
        for (auto i = m_childs.rbegin(); i != m_childs.rend(); ++i) {
            auto* ptr = &*i;
            if (fn(ptr)) {
                return ptr;
            }
        }
    }

    return nullptr;
}

int Widget::parentOrder() const
{
    if (parent())
    {
        int order = 0;
        for (auto const& c : parent()->m_childs) {
            if (&c == this) {
                return order;
            }
            ++order;
        }
    }

    return -1;
}

bool Widget::hasPrevSibling() const
{
    if (m_parent) {
        auto curr = m_parent->m_childs.citer_from_ptr(this);
        return (--curr).valid();
    }
    return false;
}

bool Widget::hasNextSibling() const
{
    if (m_parent) {
        auto curr = m_parent->m_childs.citer_from_ptr(this);
        return (++curr).valid();
    }
    return false;
}

Widget* Widget::prevSibling()
{
    if (m_parent) {
        auto curr = m_parent->m_childs.iter_from_ptr(this);
        return (--curr).valid() ? &*curr : nullptr;
    }

    return nullptr;
}

Widget* Widget::prevSibling(std::function<bool(Widget*)> const& fn)
{
    if (m_parent) {
        auto curr = m_parent->m_childs.iter_from_ptr(this);
        while ( (--curr).valid() ) {
            if (fn(&*curr))
                return &*curr;
        }
    }
    return nullptr;
}

Widget* Widget::nextSibling()
{
    if (m_parent) {
        auto curr = m_parent->m_childs.iter_from_ptr(this);
        return (++curr).valid() ? &*curr : nullptr;
    }

    return nullptr;
}

Widget* Widget::nextSibling(std::function<bool(Widget*)> const& fn)
{
    if (m_parent) {
        auto curr = m_parent->m_childs.iter_from_ptr(this);
        while ( (++curr).valid() ) {
            if (fn(&*curr))
                return &*curr;
        }
    }
    return nullptr;
}

Widget* Widget::nextTab()
{
    auto n = next(&Widget::visibleForFocus);
    while (n && !n->tabStop()) {
        n = n->next(&Widget::visibleForFocus);
    }
    return n;
}

Widget* Widget::prevTab()
{
    auto n = prev(&Widget::visibleForFocus);
    while (n && !n->tabStop()) {
        n = n->prev(&Widget::visibleForFocus);
    }
    return n;
}

Widget* Widget::next()
{
    Widget* res = mostLeftChild();

    if (!res) {
        res = nextSibling();

        if (!res) {
            res = firstNextAncle();
        }
    }

    return res;
}

Widget* Widget::next(std::function<bool(Widget*)> const& fn)
{
    Widget* res = mostLeftChild(fn);

    if (!res) {
        res = nextSibling(fn);

        if (!res) {
            res = firstNextAncle(fn); // added
        }
    }

    return res;
}

Widget* Widget::prev()
{
    Widget* res = mostRightChild();

    if (!res) {
        res = prevSibling();

        if (!res) {
            res = firstPrevAncle();
        }
    }

    return res;
}

Widget* Widget::prev(std::function<bool(Widget*)> const& fn)
{
    Widget* res = mostRightChild(fn);

    if (!res) {
        res = prevSibling(fn);

        if (!res) {
            res = firstPrevAncle(fn); // added
        }
    }

    return res;
}

Widget* Widget::mostLeftChild()
{
    Widget* res = firstChild();
    Widget* next;
    while (res && (next = res->firstChild())) {
        res = next;
    }
    return res;
}

Widget* Widget::mostLeftChild(std::function<bool(Widget*)> const& fn)
{
    Widget* res = firstChild(fn);
    Widget* next;
    while (res && (next = res->firstChild(fn))) {
        res = next;
    }
    return res;
}

Widget* Widget::mostRightChild()
{
    Widget* res = lastChild();
    Widget* next;
    while (res && (next = res->lastChild())) {
        res = next;
    }
    return res;
}

Widget* Widget::mostRightChild(std::function<bool(Widget*)> const& fn)
{
    Widget* res = lastChild(fn);
    Widget* next;
    while (res && (next = res->lastChild(fn))) {
        res = next;
    }
    return res;
}


bool Widget::hasChild(Widget const* c) const
{
    while (auto p = c->parent()) {
        if (p == this)
            return true;
    }
    return false;
}

int Widget::addAnimation(int delayMs, int durationMs, int restMs, int count)
{
    return app().addAnimation(this, delayMs, durationMs, restMs, count);
}

int Widget::resetAnimation(int animId, int delayMs, int durationMs, int restMs, int count)
{
    return app().resetAnimation(this, animId, delayMs, durationMs, restMs, count);
}

bool Widget::removeAnimation(int animationId)
{
    return app().removeAnimation(this, animationId);
}

bool Widget::holdAnimation(int animationId)
{
    return app().holdAnimation(this, animationId);
}

bool Widget::restoreAnimation(int animationId)
{
    return app().restoreAnimation(this, animationId);
}

void Widget::subscribeForIdle()
{
    app().subscribeForIdle(this);
}

void Widget::unsubscribeForIdle()
{
    app().unsubscribeForIdle(this);
}

Widget* Widget::currFocus() // static
{
    return app().focus();
}

bool Widget::focusNext() // static 
{
    auto curr = currFocus();
    Widget* f = nullptr;

    if (curr) {
        f = curr->nextTab();
        if (f) {
            f->setFocus(true);
        }
    }

    return f != nullptr;
}

bool Widget::focusPrev() // static 
{
    auto curr = currFocus();
    Widget* f = nullptr;

    if (curr) {
        f = curr->prevTab();
        if (f) {
            f->setFocus(true);
        }
    }

    return f != nullptr;
}

void Widget::dispatchPaint(GContext& ctx, GCRect const& rc)
{
    const auto c = ctx.clip();
    const auto t = ctx.translate();
    
    ctx.translate(rootOffset());
    ctx.clip(rc);

    onPaint(ctx, rc);

    ctx.clip(c);
    ctx.translate(t);
    

    // spread to childs

    for (auto& c : m_childs) {
        if (!c.isVisible())
            continue;

        auto intersection = rc.intersection(c.rect());
        if (intersection.positive()) {
            const auto offset = -c.rect().topLeft();
            intersection.moveBy(offset); // translate to children coordinates
            ctx.translateBy(offset);
            c.dispatchPaint(ctx, intersection);
        }
    }
}

GCBrush const& Widget::background() const
{
    return uistyle().brushBackground();
}

void Widget::setInited()
{
    m_state.inited(true);
}

void Widget::postConstructor()
{
    const auto sz = rect().size().applyMinSize(calcMinSize());
    const auto pt =  pointOf(rect(), m_state.anchor());
    m_rect = createRelative(pt, sz, m_state.anchor());

    /*
    // todo: fixup size

    const bool visible = reallyVisible();
    sendRepaint();
    //if (visible)
    //    onPaint(ctx, rectLocal());
    onInit(measureContext());
    setInited();
    if (visible)
        onShow(true);
    */
}

/*void Widget::fixupProperties(GContext const& ctx)
{
    auto curr_sz = m_rect.size();
    auto const min_sz = calcMinSize();
    
    curr_sz.sx = std::max(curr_sz.sx, min_sz.sx);
    curr_sz.sy = std::max(curr_sz.sy, min_sz.sy);

    m_rect = resize(m_rect, curr_sz, anchor());
}*/

GCBrush const& Widget::parentBackground() const
{
    return parent() ? parent()->background() : uistyle().brushBackground();
}

void Widget::notifyShow(bool show)
{
    onShow(show);

    for (auto& c : m_childs) {
        if (c.isVisible())
            c.notifyShow(show);
    }
}

void Widget::postRepaint()
{
    app().postMessage(this, MsgPaint{rectLocal()});
}

void Widget::postRepaint(GCRect const& rc)
{
    app().postMessage(this, MsgPaint{rc});
}

void Widget::sendRepaint()
{
    app().sendMessage(this, MsgPaint{rectLocal()});
}

void Widget::sendRepaint(GCRect const& rc)
{
    app().sendMessage(this, MsgPaint{rc});
}

GContext const& Widget::measureContext() // static
{
    return app().context();
}
