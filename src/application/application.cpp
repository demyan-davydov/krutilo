// -*- coding:utf-8-unix -*-
/*!
    @file       application.cpp
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

#include "application.h"
#include "application_keys.h"

#include "device_ili9341.h"
#include "surface_ili9341.h"
#include "keypad.h"
#include "stepper_pwm.h"
/*
#include "widget/widget_main.h"
#include "widget/widget_panel.h"
#include "widget/widget_label.h"
*/
#include "fonts/PT_Mono_13.h"
#include "widget/ui_style.h"

#include <algorithm> // find


Application::Application()
: m_displayDevice(PA_1, PA_4, SPI_MOSI, SPI_SCK, SPI_MISO, NC)
, m_surface(m_displayDevice)
, m_stepper(B5, B8, B4, 200*16)
, m_keypad({A8, A9, A10, A11}, {B15, B14, B13, B12})
{
    m_surface.orientation(Surface_ILI9341::Orientation::Orient_270CW);
    m_timer.start();
    m_stepper.hardStopFlag(false);
}

Application::~Application()
{

}

bool Application::run()
{
    m_keypad.init(m_timer.read_ms());

    while(!canceled())
    {
        const timestamp_t curr_time = m_timer.read_ms();

        // process keypad events
        if (focus())
        {
            const char* alphabet = k_keypad_alphabet;

            m_keypad.update(curr_time);
            KeypadState::EventStream kpEvents(m_keypad.state());
            while (!kpEvents.empty()) {
                auto e = kpEvents.pop();
                if (e.status == KeypadState::PRESSED) {
                    dispatchMsg(MsgKeyDown(alphabet[e.idx]), nullptr, focus());
                }
                else if (e.status == KeypadState::RELEASED) {
                    dispatchMsg(MsgKeyUp{alphabet[e.idx]}, nullptr, focus());
                }
                break;
            }
        }

        // process regular events
        if (!m_msgQueue.empty()) {
            auto& wrapped = m_msgQueue.front();
            auto& m = *wrapped.msg.get();
            handleMessage(m, wrapped.src, wrapped.dst);
            m_msgQueue.pop();
        }

        // process animations
        {
            auto t = m_timer.read_ms();
            m_anim.enumerate(t, [this](void* owner, int animId, float progress, int count){
                Widget* w = (Widget*)owner;
                handleAnimation(w, animId, progress, count);
            });

            m_anim.removeEstimined(t, [](void* owner, int animId) {
                Widget* w = (Widget*)owner;
                w->onAnimationFinished(animId);
            });
        }

        // notify idle
        {
            for (auto w : m_idleSubscribers) {
                if (w->isInited())
                    w->onIdle();
            }
        }
    }

    return true;
}

GCRect Application::screenRect() const
{
    return m_surface.screenRect();
}

Widget* Application::focus()
{
    return (Widget*)m_focusWidget;
}

void Application::showSplash()
{
    m_surface.drawCircle({100,100}, 60);
}

void Application::handleMessage(Msg const& m, Widget* src, Widget* dst)
{
    switch (m.kind()) {
    case MSG_Init:      dispatchMsg((MsgInit&)m, src, dst); break;
    case MSG_KeyUp:     dispatchMsg((MsgKeyUp&)m, src, dst); break;
    case MSG_KeyDown:   dispatchMsg((MsgKeyDown&)m, src, dst); break;
    case MSG_Paint:     dispatchMsg((MsgPaint&)m, src, dst); break;

    default:
        break;
    }
}

GContext const& Application::context() const
{
    return m_surface;
}

void Application::setFocusWidget(Widget* w)
{
    m_focusWidget = w;
}

Widget* Application::getFocusWidget()
{
    return (Widget*)m_focusWidget;
}

int Application::addAnimation(Widget* w, int delayMs, int durationMs, int restMs, int count)
{
    return m_anim.add(w, m_timer.read_ms() + delayMs, durationMs, restMs, count);
}

int Application::resetAnimation(Widget* w, int animId, int delayMs, int durationMs, int restMs, int count)
{
    return m_anim.reset(w, animId, m_timer.read_ms() + delayMs, durationMs, restMs, count);
}

bool Application::removeAnimation(Widget* w, int animationId)
{
    return m_anim.remove(w, animationId);
}

bool Application::holdAnimation(Widget* w, int animationId)
{
    return m_anim.hold(w, animationId);
}

bool Application::restoreAnimation(Widget* w, int animationId)
{
    return m_anim.restore(w, animationId);
}

void Application::subscribeForIdle(Widget* w)
{
    if (w) {
        auto i = std::find(m_idleSubscribers.begin(), m_idleSubscribers.end(), w);
        if (i == m_idleSubscribers.end()) {
            m_idleSubscribers.push_back(w);
        }
    }
}

void Application::unsubscribeForIdle(Widget* w)
{
    auto i = std::find(m_idleSubscribers.begin(), m_idleSubscribers.end(), w);
    if (i != m_idleSubscribers.end()) {
        m_idleSubscribers.erase(i);
    }
}

void Application::widgetRemoved(Widget* w)
{
    m_msgQueue.removeByDestination(w);
    m_anim.removeByOwner(w);
    unsubscribeForIdle(w);

    if (m_focusWidget == w) {
        m_focusWidget = nullptr;
    }
}
