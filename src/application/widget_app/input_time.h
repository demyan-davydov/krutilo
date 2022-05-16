// -*- coding:utf-8-unix -*-
/*!
    @file       input_time.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-25

    @brief      Time input widget
    @description

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__WIDGET_INPUT_TIME_H
#define file__WIDGET_INPUT_TIME_H

#include "../widget/widget.h"
#include "anim_helper_cursor_fade.h"
#include "labels_win1251.h"

class InputTime : public Widget
{
    static constexpr const char* k_str_hours = label_hours_short;// "h";
    static constexpr const char* k_str_minutes = label_minutes_short;// "m";

    static constexpr uint8_t k_hours_digits = 2;
    static constexpr uint8_t k_minutes_digits = 2;
    static constexpr uint8_t k_total_digits = k_hours_digits + k_minutes_digits;
    static constexpr uint8_t k_gap = 1;
    static constexpr uint8_t k_corner_radius = 3;

public:
    InputTime(Widget* parent, GContext const& ctx, GCPoint const& pt, int width);


    void onPaint(GContext& ctx, GCRect const& rc) override;

    void onAnimation(int animId, float progress, int count) override;

    void onAnimationFinished(int animId) override;

    void onShow(bool show) override;

    void onFocus(bool show) override;

    bool onBubbling(MsgKeyDown const&, Widget const* target) override;

    bool onBubbling(MsgKeyUp const&, Widget const* target) override;

    //bool tabStop() const override { return true; } // can be focused

    uint32_t totalMinutes() const;

    uint32_t totalSeconds() const;

private:
    GCRect cursorRect(GContext const& ctx) const;

    void drawCursor(GContext& ctx);

    bool replaceCurrDigit(uint8_t digit);

    void moveCursorLeft();

    void moveCursorRight();

    //static GCPoint drawStr(GContext& ctx, GCPoint const& pt, const char* txt, int posCursor);

    static bool isInputChar(char c);

    static GCSize calcMinSize(GContext const& ctx);

    static GCRect calcRect(GCCoord width, GCPoint const& pt, GContext const& ctx);

    static GCExtents2d extents() {return GCExtents2d{{5,5}, {3,3}}; }

    uint8_t m_pos = 0; // cursor pos, from most right position
    uint16_t m_minutes = 0;
    uint16_t m_hours = 0;
    AnimHelperCursorFade m_animHelper;
    bool m_pressed = false;
};

#endif // file__WIDGET_INPUT_TIME_H
