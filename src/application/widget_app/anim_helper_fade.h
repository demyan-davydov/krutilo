// -*- coding:utf-8-unix -*-
/*!
    @file       anim_helper_fade.h
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

#include <cstdint>

#pragma once
#ifndef file__ANIM_HELPER_FADE_H
#define file__ANIM_HELPER_FADE_H

class Widget;

class AnimHelperFade
{
public:
    void onAnimation(int animId, float progress);
    void onAnimationFinished(int animId);
    void onShow(Widget& w, bool show);
    void onFocus(Widget& w, bool f);

    uint16_t bgColor(Widget const& w) const;

private:
    int m_animIdFade = 0;    ///< fade in/out animation ID
    float m_fade_progress = 1;
};

#endif // file__ANIM_HELPER_FADE_H
