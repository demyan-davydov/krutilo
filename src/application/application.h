// -*- coding:utf-8-unix -*-
/*!
    @file       application.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Application
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__APPLICATION_H
#define file__APPLICATION_H

#include "geo/gc_rect.h"

#include "widget/widget.h"
#include "message_queue.h"
#include "animation.h"
#include "context.h"

#include "device_ili9341.h"
#include "surface_ili9341.h"
#include "keypad.h"
#include "stepper_pwm.h"

#include <list>

#include <mbed.h>


#include "brush_solid.h"


class Widget;

class Application
{
    friend class Widget; // Really need this?

    Application(Application const&) = delete;
    Application& operator=(Application const&) = delete;

private:
    /// forbid to create Application immediatelly,
    /// use instance()
    Application(); 
    ~Application();

public:
    static Application& instance()
    {
        static Application s;
        return s;
    }

    /// show splash intro
    void showSplash();

    /// run application message loop
    bool run();

    GCRect screenRect() const;

    StepperPwm& stepper() { return m_stepper; }

    StepperPwm const& stepper() const { return m_stepper; }

private:
    bool canceled() const { return false; }

    Widget* focus();


    /// dispatch ordinal message throught parent..target path
    /// Bubbling/Singing approach used 
    /// onBubbling, we notify all the parents of target widget
    /// onSinking we notify parents in backward order
    ///  
    template<typename TMsg>
    void dispatchMsgByPath(TMsg const& m, Widget** path, int16_t path_size)
    { 
        // do bubbling
        int16_t idx = 0;
        for (; idx < path_size; ++idx) {
            if (!path[idx]->onBubbling(m, path[idx])) {
                ++idx;
                break;
            }
        }

        // do sinking
        for (--idx; idx >= 0; --idx) {
            if (!path[idx]->onSinking(m, path[idx]))
                break;
        }
    }

    /// dispatch ordinal message
    /// Bubbling/Singing approach used 
    /// onBubbling, we notify all the parents of target widget
    /// onSinking we notify parents in backward order
    ///  
    template<typename TMsg>
    void dispatchMsg(TMsg const& m, Widget* sender, Widget* target)
    { 
        if (target) {
            //std::vector<Widget*> path(path_size);
            const uint16_t path_size = target->deep();
            std::unique_ptr<Widget*[]> path(new Widget*[path_size]);
            {
                uint16_t idx = path_size;
                auto p = target;
                do {
                    path[--idx] = p;
                } while ((p = p->parent()));
            }

            dispatchMsgByPath(m, path.get(), (int16_t)path_size);
        }
    }

    /// dispath Paint message
    /// onPaint called only for visible widgets
    void dispatchMsg(MsgPaint const& m, Widget* sender, Widget* target)
    {
        if (target && target->reallyVisible()) {
            GContext& ctx = m_surface;
            ctx.translate(target->rootOffset());
            target->dispatchPaint(ctx, m.rc);
        }
    }

    /// dispath 'Init' message
    /// here is sequence of nitifications:
    ///   onPaint()
    ///   onInit()
    ///   onShow()
    void dispatchMsg(MsgInit const& m, Widget* sender, Widget* target)
    {
        if (target) {
            GContext& ctx = m_surface;

            //auto c = ctx.clip(); // save clip
            //auto t = ctx.translate(); // save translate
            ctx.translate(target->rootOffset());
            ctx.clip(target->rectLocal());
            
            const bool visible = target->reallyVisible();
            if (visible)
                target->onPaint(ctx, target->rectLocal());
            target->onInit(ctx);
            target->setInited();
            if (visible)
                target->onShow(true);

            //ctx.clip(c); // restore clip
            //ctx.translate(t); // restore translate
        }
    }

    /// call onAnimation for target widget
    void handleAnimation(Widget* target, int animId, float progress, int count)
    {
        if (target->reallyVisible()) {
            target->onAnimation(animId, progress, count);
        }
    }

    /// methods called by window
    void handleMessage(Msg const& msg, Widget* src, Widget* dst);

    /// get context for measure purposes
    GContext const& context() const;

    /// add message to message queue
    template<typename TMsg>
    void postMessage(Widget* destination, TMsg const& m)
    {
        if (destination) {
            m_msgQueue.append(nullptr, destination, m);
        }
    }

    /// process message immediatelly
    template<typename TMsg>
    void sendMessage(Widget* destination, TMsg const& m)
    {
        if (destination) {
            dispatchMsg(m, nullptr, destination);
        }
    }

    void setFocusWidget(Widget*);

    Widget* getFocusWidget();

    /// Animation API

    int addAnimation(Widget* w, int delayMs, int durationMs, int restMs, int count=-1);

    int resetAnimation(Widget* w, int animId, int delayMs, int durationMs, int restMs, int count=-1);

    bool removeAnimation(Widget* w, int animationId);

    bool holdAnimation(Widget* w, int animationId);

    bool restoreAnimation(Widget* w, int animationId);

    /// Idle subscibers
    void subscribeForIdle(Widget*);

    void unsubscribeForIdle(Widget*);

    // 

    /// action on widget removed
    void widgetRemoved(Widget* w);


    GContext& ctx() { return m_surface; }

    GContext const& ctx() const { return m_surface; }

private:
    Device_ILI9341 m_displayDevice;
    Surface_ILI9341 m_surface;
    StepperPwm m_stepper;
    Keypad m_keypad;
    MessageQueue m_msgQueue;
    std::vector<Widget*> m_idleSubscribers;
    Animation m_anim;
    Timer m_timer;
    void* m_focusWidget = nullptr;
};

/// short pseudoname for Application::instance()
inline Application& app() { return Application::instance(); }

/// don't really needed, but still it here
//static void app_stop() { Application::instanceFree(); }


#endif // file__APPLICATION_H
