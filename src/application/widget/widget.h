// -*- coding:utf-8-unix -*-
/*!
    @file       widget.h
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

#pragma once
#ifndef file__WIDGET_H
#define file__WIDGET_H

#include "gc_rect.h"
#include "anchor.h"
#include "../context.h"
#include "../message.h"
#include "ui_state.h"

#include <list>
#include <functional>
#include "../var_list.h"


class Application;

class Widget
{
    friend class Application; // for setInited()

    typedef Widget self_t;

protected:
    /// Hidden: prevent direct Widget creation

    //class Hidden {};

    //Widget* hidenParent() { return reinterpret_cast<Widget*>(parent()); }

    //static Widget* unhide(Widget* w) { return reinterpret_cast<Widget*>(w); }

public:
    typedef bool (*WidgetFilterFn)(Widget*); 

    //typedef KeypadState::Status KeyStatus;

    enum class Checkable : uint8_t 
    {
        Not,
        Radiobutton,
        Checkbox,
    };

    enum class StateShift : uint8_t {
        Visible     = 0,    // inherited
        Enabled     = 1,    // inherited
        Pressed     = 2,
        Checked     = 3, // keep one more bit after, checked state may be one of 3

        NoRepaint   = 5, // block repaint
        Inited      = 6, // onInit was called
        TabStop     = 7, // is tabstop widget (can be focused)

        Anchor      = 12, // anchor (4 bits)

        //Next,       = 5, ///
    };

    enum class StateMask : uint16_t {
        Visible     = 1 << (uint8_t)StateShift::Visible,
        Enabled     = 1 << (uint8_t)StateShift::Enabled,
        Pressed     = 1 << (uint8_t)StateShift::Pressed,
        Checked     = 1 << (uint8_t)StateShift::Checked,
        NoRepaint   = 1 << (uint8_t)StateShift::NoRepaint,
        Inited      = 1 << (uint8_t)StateShift::Inited,
        TabStop     = 1 << (uint8_t)StateShift::TabStop,

        Anchor      = (1+2+4+8) << (uint8_t)StateShift::Anchor,
    };

    struct State
    {
        static constexpr uint16_t k_default = (uint16_t)StateMask::Visible | (uint16_t)StateMask::Enabled;
        static constexpr uint16_t k_default_tabstop = k_default | (uint16_t)StateMask::TabStop;

        State(uint16_t st) : m_flags(st) {}
        State(){}

        bool visible() const { return getBit(StateMask::Visible); }
        bool enabled() const { return getBit(StateMask::Enabled); }
        bool pressed() const { return getBit(StateMask::Pressed); }
        bool checked() const { return getBit(StateMask::Checked); }
        bool noRepaint() const { return getBit(StateMask::NoRepaint); }
        bool inited() const { return getBit(StateMask::Inited); }
        bool tabStop() const { return getBit(StateMask::TabStop); }
        Anchor anchor() const { return (Anchor)(m_flags >> (uint8_t)(StateShift::Anchor)); }

        void visible(bool v) { setBit(StateMask::Visible, v); }
        void enabled(bool v) { setBit(StateMask::Enabled, v); }
        void pressed(bool v) { setBit(StateMask::Pressed, v); }
        void checked(bool v) { setBit(StateMask::Checked, v); }
        void noRepaint(bool v) { setBit(StateMask::NoRepaint, v); }
        void inited(bool v) { setBit(StateMask::Inited, v); }
        void tabStop(bool v) { setBit(StateMask::TabStop, v); }
        void anchor(Anchor v) { m_flags &= ~(uint16_t)StateMask::Anchor; m_flags |= (uint16_t)v << (uint8_t)StateShift::Anchor; }

    private:
        bool getBit(StateMask mask) const { return m_flags & (uint8_t)mask; }
        void setBit(StateMask mask, bool v) {
            if (v) { m_flags |= (uint8_t)mask; }
            else   { m_flags &= ~(uint8_t)mask; }
        }

        uint16_t m_flags = k_default;// (uint16_t)StateMask::Visible | (uint16_t)StateMask::Enabled;
    };

public:

protected:
    explicit Widget(Widget* parent, GCRect const& rc, const State& initState = State{});

    explicit Widget(Widget* parent, const State& initState = State{});

    explicit Widget(Widget* parent, GCPoint const& pt, GCSize const& sz, Anchor a, const State& initState = State{});

public:
    template<typename TWidget, typename... Args>
    static TWidget* createRoot(Args&&... args)
    {
        static_assert(std::is_base_of<Widget, TWidget>::value, "can create only classes based on Widget");

        TWidget* w = new TWidget(nullptr, std::forward<Args>(args)...);

        w->postConstructor();

        return w;
    }
/*
    static Widget* create(Widget* parent, const State& initState = State{});

    static Widget* create(Widget* parent, GCRect const& rc, const State& initState = State{});

    static Widget* create(Widget* parent, GCPoint const& pt, GCSize const& sz, Anchor a, const State& initState = State{});
*/
    virtual ~Widget();

    //int uistate() const;

    /// @brief number of precending parents + 1
    /// if Widget has no parents return 1;
    uint16_t deep() const;

    /// rect in parent coordiantes
    GCRect const& rect() const;

    /// widget size
    GCSize size() const;

    /// rect in local coordinates, top-left corner is {0,0}
    GCRect rectLocal() const;

    /// offset left-top relative to root element
    GCPoint rootOffset() const;

    /// set focus, repaint tself or parent iif needed 
    void setFocus(bool focus);

    /// element has focus
    bool hasFocus() const; 

    /// 
    bool isChecked() const; 

    /// set element checked, has effect only for Checkbox and radio buttons
    void setChecked(bool v); 

    /// 
    void toggleChecked();

    ///
    void setVisible(bool visible, bool repaintParent = true);

    ///
    bool isVisible() const;

    /// 
    bool reallyVisible() const; ///< visible and all parent also visible

    /// 
    void setEnabled(bool enable);

    ///
    bool isEnabled() const; 

    /// 
    bool reallyEnabled() const; ///< enabled and all parent also enabled

    /// 
    bool isInited() const;

    /// @return true if repaint blocked
    bool isRepaintBlocked() const;

    /// @brief block repaint
    void noRepaint(bool v);

    /// @brief return resize anchor
    Anchor anchor() const;

    /// @brief set resize anchor
    void anchor(Anchor a);

    /// set rect in coordinates of parent, relative to parent
    void setRect(GCRect const& newRc );

    /// set new size relative to anchor
    void setSize(GCSize const& newSize);


    // child, parent, sibling enumeration
    
    /// return number of childs
    uint32_t childCount() const;

    /// return n'th child of this widget, or null
    Widget* nthChild(uint32_t n);

    Widget* root();

    Widget* parent() { return m_parent; };

    Widget const* parent() const { return m_parent; };

    Widget* firstParent(std::function<bool(Widget*)> const& fn);

    Widget* nextAncle();

    Widget* prevAncle();

    Widget* firstNextAncle();

    Widget* firstNextAncle(std::function<bool(Widget*)> const& fn);

    Widget* firstPrevAncle();

    Widget* firstPrevAncle(std::function<bool(Widget*)> const& fn);

    Widget* firstChild();

    Widget* firstChild(std::function<bool(Widget*)> const& fn);

    Widget* lastChild();

    Widget* lastChild(std::function<bool(Widget*)> const& fn);

    /// return order of this widget in parent childs
    /// or -1 if has no parent
    int parentOrder() const;

    bool hasPrevSibling() const;

    bool hasNextSibling() const;

    Widget* prevSibling();

    Widget* prevSibling(std::function<bool(Widget*)> const& fn);

    Widget* nextSibling();

    Widget* nextSibling(std::function<bool(Widget*)> const& fn);

    Widget* nextTab(); ///< next window with tabStop

    Widget* prevTab(); ///< previous window with tabStop

    Widget* next(); ///< previous widget

    Widget* next(std::function<bool(Widget*)> const& fn); ///< previous widget

    Widget* prev(); ///< previous widget

    Widget* prev(std::function<bool(Widget*)> const& fn); ///< previous widget

    Widget* mostLeftChild(); ///< most left child

    Widget* mostLeftChild(std::function<bool(Widget*)> const& fn); ///< most left child for that fn(child) == true

    Widget* mostRightChild(); ///< most right child

    Widget* mostRightChild(std::function<bool(Widget*)> const& fn); ///< most right child for that fn(child) == true


    /// 
    bool hasChild(Widget const*) const;

    /// add animation
    /// @return animatonId
    int addAnimation(int delayMs, int durationMs, int restMs, int count=-1);

    /// if animation with specified id is exists - remove it
    /// and new animation with specified parameters
    /// @return new animatonId
    int resetAnimation(int animId, int delayMs, int durationMs, int restMs, int count=-1);

    /// remove animation with specified id
    bool removeAnimation(int animationId);

    /// pause animation
    bool holdAnimation(int animationId);

    /// pause animation
    bool restoreAnimation(int animationId);

    /// subscribe for Idle events
    /// it safe to subscribe few times, nofing happened on second attempt to subscribe
    void subscribeForIdle();

    /// unsubscribe from Idle events
    /// it safe to unsubscribe if you not subscribed
    void unsubscribeForIdle();

    /// return currrently focused widget
    static Widget* currFocus();

    /// focus next control (if possible) 
    static bool focusNext();

    /// focus previous control (if possible)
    static bool focusPrev();

    // create new child 

    //void appendChild(std::unique_ptr<Widget>&& child);

    /// append new child
    template<typename TWidget, typename... Args>
    TWidget& append(Args&&... args)
    {
        auto& w = m_childs.emplace_back<TWidget>(reinterpret_cast<Widget*>(this), std::forward<Args>(args)...);

        w.postConstructor();

        return w;
    }

    /// @brief call onPaint() for widget and all of it visible childrens
    void dispatchPaint(GContext& ctx, GCRect const& rc);

    /// @return widget state flags
    State const& state() const { return m_state; }

    /// element can be focused
    //virtual bool tabStop() const { return false; }
    bool tabStop() const { return m_state.tabStop(); }

    /// element can be checked (checkbox, radiobutton)
    //virtual Checkable checkable() const { return Checkable::Not; }
    /// Todo: must be virtual, but virtualtiti removed to decrease binary size
    Checkable checkable() const { return Checkable::Not; }

    /// element can be pressed
    //virtual bool pressable() const { return false; }


    // Notifications

    /// Called ommediately after element creation
    virtual void onInit(GContext const& ctx) {}

    /// Called then widget need to be repaint
    virtual void onPaint(GContext& ctx, GCRect const& rc) {}

    /// Called then element aquared or lost focus, before corresponding repaint
    virtual void onFocus(bool acquired) {}

    /// called before visible state is changed
    virtual void onShow(bool show) {}

    /// called then message loop is in IDLE state, 
    /// You need to subscribe for idle to recieve this notifications
    virtual void onIdle() {}

    /// On animation 
    virtual void onAnimation(int animId, float progress, int count) {}
    virtual void onAnimationFinished(int animId) {}

    /// on message bubbling
    /// you need to check 'target' field to determine target of message
    /// return true if you want to continue bubbling, or false to prevent bubbling and start sinking 
    virtual bool onBubbling(MsgKeyUp const&, Widget const* target) { return true; }

    /// on message sinking
    /// return false to stop sinking
    virtual bool onSinking(MsgKeyUp const&, Widget const* target) { return true; }


    virtual bool onBubbling(MsgKeyDown const&, Widget const* target) { return true; }
    virtual bool onSinking(MsgKeyDown const&, Widget const* target) { return true; }

    /// calculate minimum size of widget
    virtual GCSize calcMinSize() const { return GCSize{0,0}; }

    /// return background brush
    /// may be used by child controls who wants to draw 'transparent' background
    virtual GCBrush const& background() const;

private:
    /// set 'initialized' flag
    /// called immediatelly after onInit() call
    void setInited();

    /// call immediatelly after creation
    void postConstructor();

protected:
    // called immediatelly before onInit()
    //void fixupProperties(GContext const& ctx);

    /// additional check for widget what can to be focused
    static bool visibleForFocus(Widget* w) { return w->reallyVisible() && w->isEnabled(); /*&& w->tabStop()*/ }

    // return this method from background() to implement preudo transparent background
    GCBrush const& parentBackground() const;

protected:
    /// spread onShow() notification for widget ant its children
    void notifyShow(bool show);

    /// post repaint request to message queue
    void postRepaint();

    /// post repaint request to message queue
    void postRepaint(GCRect const& rc);

    /// repaint widget now if possible
    void sendRepaint();

    /// repaint widget now if possible
    void sendRepaint(GCRect const& rc);

    /// get context for measure purposes
    static GContext const& measureContext(); 

protected:
    Widget* m_parent = nullptr; ///< parent widget, null for root widget
    State m_state; ///< local state, really value inherited
    GCRect m_rect = {-1,-1}; ///< local value, rect relative to parent, really value inherited
    var_list<Widget> m_childs; ///< list of childs items
};

#endif // file__WIDGET_H
