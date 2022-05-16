// -*- coding:utf-8-unix -*-
/*!
    @file       anim_helper_cursor_fade.cpp
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-26

    @brief      Animation Helper for fade in/out inputs with cursor
    @description

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#include "anim_helper_cursor_fade.h"
#include "../widget/widget.h"
#include "../widget/ui_style.h"


void AnimHelperCursorFade::onAnimation(int animId, float p)
{
    if (animId == m_animIdCursor) {
        m_blink = !m_blink;
    }
    else 
        m_fade.onAnimation(animId, p);
}

void AnimHelperCursorFade::onAnimationFinished(int animId)
{
    if (animId == m_animIdCursor) {
        m_animIdCursor = 0;
    }
    else 
        m_fade.onAnimationFinished(animId);
}

void AnimHelperCursorFade::onShow(Widget& w, bool show)
{
    m_fade.onShow(w, show);

    if (show && w.hasFocus()) {
        w.restoreAnimation(m_animIdCursor);
    }
    else {
        w.holdAnimation(m_animIdCursor);
    }
}

void AnimHelperCursorFade::onFocus(Widget& w, bool f)
{
    m_fade.onFocus(w, f);

    if (f) {
        m_blink = false;
        m_animIdCursor = w.resetAnimation(m_animIdCursor, 300, 0, 600);
    }
    else {
        w.holdAnimation(m_animIdCursor);
    }
}

uint16_t AnimHelperCursorFade::bgColor(Widget const& w) const
{
    return m_fade.bgColor(w);
}

uint16_t AnimHelperCursorFade::pressedBg() const
{
    return Clr_YELLOW;
}