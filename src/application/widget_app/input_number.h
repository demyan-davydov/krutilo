// -*- coding:utf-8-unix -*-
/*!
    @file       input_number.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-25

    @brief      Input for numbers
    @description

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__WIDGET_INPUT_NUMBER_H
#define file__WIDGET_INPUT_NUMBER_H

#include "../widget/widget.h"
#include <string>

#include "anim_helper_fade.h"
#include "anim_helper_cursor_fade.h"


class InputNumber : public Widget
{
    static constexpr uint8_t k_max_digits = 7;
    static constexpr uint8_t k_gap = 2; // gap between cursor and last digit
    static constexpr uint8_t k_corner_radius = 3;
    static constexpr uint8_t k_cursor_w = 2;

public:
    InputNumber(Widget* parent, GContext const& ctx, GCPoint const& pt, int width);

    void onPaint(GContext& ctx, GCRect const& rc) override;

    void onAnimation(int animId, float progress, int count) override;

    void onAnimationFinished(int animId) override;

    void onShow(bool show) override;

    void onFocus(bool show) override;

    bool onBubbling(MsgKeyDown const&, Widget const* target) override;

    bool onBubbling(MsgKeyUp const&, Widget const* target) override;

    //bool tabStop() const override { return true; } // can be focused

    uint32_t value() const;

private:
    GCRect cursorRect(GContext& ctx) const;

    void drawCursor(GContext& ctx);

    void appendDigit(char v);

    void eraseLastDigit();

    uint8_t digitsCount() const;

    static bool isInputChar(char c);

    static GCSize calcMinSize(GContext const& ctx);

    static GCRect calcRect(GCCoord width, GCPoint const& pt, GContext const& ctx);

    static GCExtents2d extents() {return GCExtents2d{{5,5}, {3,3}}; }

    uint32_t m_value = 0;    ///< current value    
    AnimHelperCursorFade m_animHelper;
    bool m_pressed = false;  ///< keypad pressed
    char m_pressed_char = 0; ///< current input char
};

#endif // file__WIDGET_INPUT_NUMBER_H
