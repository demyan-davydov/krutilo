    // -*- coding:utf-8-unix -*-
/*!
    @file       widget_progress.cpp
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

#include "widget_progress.h"
#include "ui_style.h"
#include "brush/brush_strip.h"
#include "brush/brush_strip_horz.h"
#include "math_util.h"

//#include <cmath> // M_PI, sin()


    
WidgetProgressBar::WidgetProgressBar(Widget* parent, GCRect const& rc)
: base_t(parent, rc)
{

}

static int fillWidth(GCRect const& rc, float value)
{
    const int fullW = rc.width() - 2;
    return fullW * value;
}

static void splitRect(GCRect const& rc, GCRect& rcFill, GCRect& rcEmpty, float value)
{
    rcFill = rcEmpty = rc.decreasedBy(GCSize{1,1});
    const int w = rcFill.width();
    rcFill.rx.resizeFromA(w*value);
    rcEmpty.rx.a = rcFill.rx.b + 1;
}

float WidgetProgressBar::value() const
{
    return m_value;
}

void WidgetProgressBar::value(float v)
{
    if (v < 0) 
        v = 0;
    else if (v > 1)
        v = 1;

    m_value = v;

    auto newBarWidth = fillWidth(rect(), v);
    if (newBarWidth != m_prevBarWidth) {
        m_prevBarWidth = newBarWidth;
        //sendRepaint();
    }
}

void WidgetProgressBar::state(State s)
{
    m_state = s;
    switch (s) {
        case State::Finished:
        case State::Progressed: m_animId = base_t::resetAnimation(m_animId, 0, 1000, 0); 
        break;
        case State::Paused: m_animId = base_t::resetAnimation(m_animId, 0, 1500, 0); break;
    }
    // sendRepaint();
}

void WidgetProgressBar::onInit(GContext const& ctx)
{
    m_animId = base_t::addAnimation(0, 1000, 0);
}

void WidgetProgressBar::onPaint(GContext& ctx, GCRect const& rc)
{
    constexpr auto c1 = Clr_NAVY;
    constexpr auto c2 = Clr_ROYAL_BLUE;
    constexpr int w1 = 12;
    constexpr int w2 = 12;
    //constexpr int w_period = w1 + w2;

    GCRect rcFill{0,0,0,0};
    GCRect rcEmpty{0,0,0,0};
    splitRect(rc, rcFill, rcEmpty, m_value);

    GCBrushSolid brushBorder(Clr_DARK_GRAY);
    GCBrushSolid brushEmpty(Clr_GRAY);

    // draw border
    ctx.drawRect(rc, brushBorder);
    // draw empty
    ctx.fillRect(rcEmpty, brushEmpty);

    if (m_state == State::Paused) {
        //float progress = m_anim_progress - 0.2;
        //if (progress < 0) progress = 0;
        //else progress *= 1.f / (1.f - 0.2f);

        int ww1 = (2 * (sinx(m_anim_progress*my_PIx2)+1.f) + 0.5)*2 + 8;
        int ww2 = 24 - ww1;

        GCBrushStripHorz brush(c1, c2, ww1, ww2);
        brush.setOffset({-ww1/2, 0});
        ctx.fillRect(rcFill, brush);
    }
    else if (m_state == State::Finished) {
        constexpr int ww1 = 12;
        constexpr int ww2 = 12;

        GCBrushStripHorz brush(c1, c2, ww1, ww2);
        //brush.setOffset({-ww1/2, 0});
        ctx.fillRect(rcFill, brush);
    }
    else if (m_state == State::Progressed){
        GCBrushStrip brush(c1, c2, w1, w2);
        int shift = m_anim_progress * 24;
        brush.setOffset({shift + rcFill.width(), 0});
        ctx.fillRect(rcFill, brush);
    }
}

void WidgetProgressBar::onShow(bool show)
{
    if (show) 
        base_t::restoreAnimation(m_animId);
    else
        base_t::holdAnimation(m_animId);
}

void WidgetProgressBar::onAnimation(int animId, float progress, int count)
{
    m_anim_progress = progress;
    sendRepaint();
}

void WidgetProgressBar::onAnimationFinished(int animId)
{
    if (m_animId == animId) {
        m_animId = 0;
    }
}
