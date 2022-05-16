// -*- coding:utf-8-unix -*-
/*!
    @file       widget_progress.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Progress bar
    @description
        Progress bar with 3 states: progressed, paused, finished

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__WIDGET_PROGRESSBAR_H
#define file__WIDGET_PROGRESSBAR_H

#include "widget.h"
#include "gc_rect.h"
#include "ui_style.h"

class WidgetProgressBar : public Widget
{
    typedef Widget base_t;

public:
    enum class State : uint8_t
    {
        Progressed,
        Paused,
        Finished,
    };

    WidgetProgressBar(Widget* parent, GCRect const& rc);

    /// return progress value [0..1]
    float value() const;

    /// set value [0..1]
    void value(float v);

    /// 
    State state() const { return m_state; }

    /// 
    void state(State s);

protected:
    void onInit(GContext const& ctx) override;
    void onPaint(GContext& ctx, GCRect const& rc) override;
    void onShow(bool show) override;
    void onAnimation(int animId, float progress, int count) override;
    void onAnimationFinished(int animId) override;

private:
    int m_animId = 0;
    int m_prevBarWidth = 0; // used to reduce repaints
    State m_state = State::Progressed;
    float m_value = 0;
    float m_anim_progress = 0;
};


#endif // file__WIDGET_PROGRESSBAR_H

