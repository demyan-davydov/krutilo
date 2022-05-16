// -*- coding:utf-8-unix -*-
/*!
    @file       anim_helper_cursor_fade.h
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

#include "anim_helper_fade.h"

#include <cstdint>

#pragma once
#ifndef file__ANIM_HELPER_CURSOR_FADE_H
#define file__ANIM_HELPER_CURSOR_FADE_H

class Widget;

class AnimHelperCursorFade //: public WidgetMixin
{
public:
    void onAnimation(int animId, float progress);
    void onAnimationFinished(int animId);
    void onShow(Widget& w, bool show);
    void onFocus(Widget& w, bool f);

    uint16_t bgColor(Widget const& w) const;
    uint16_t pressedBg() const;
    bool blink() const { return m_blink; }

private:
    int m_animIdCursor = 0; ///< cursor blinking animation ID
    bool m_blink;           ///< is cursor blinked now 
    AnimHelperFade m_fade;  ///< fade in/out animations
};

#endif // file__ANIM_HELPER_CURSOR_FADE_H
