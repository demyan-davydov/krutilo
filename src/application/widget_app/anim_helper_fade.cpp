// -*- coding:utf-8-unix -*-
/*!
    @file       anim_helper_fade.cpp
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-25

    @brief      Helper for fade in/out inputs
    @description

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/


#include "anim_helper_fade.h"
#include "../widget/widget.h"
#include "../widget/ui_style.h"



void AnimHelperFade::onAnimation(int animId, float p)
{
    if (animId == m_animIdFade) {
        m_fade_progress = p;
    }
}

void AnimHelperFade::onAnimationFinished(int animId)
{
    if (animId == m_animIdFade) {
        m_animIdFade = 0;
    }
}

void AnimHelperFade::onShow(Widget& w, bool show)
{
    if (show) {
        m_fade_progress = 1;
    }
}

void AnimHelperFade::onFocus(Widget& w, bool f)
{
    m_fade_progress = 0;
    m_animIdFade = w.resetAnimation(m_animIdFade, 0, 300, 0, 1);
}

uint16_t AnimHelperFade::bgColor(Widget const& w) const
{
    return uistyle().getInputBgColor(w.hasFocus(), m_fade_progress);
}