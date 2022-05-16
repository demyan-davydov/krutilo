// -*- coding:utf-8-unix -*-
/*!
    @file       smooth_label.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Smooth appearing label
    @description
        Show text content

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__SMOOTH_LABEL_H
#define file__SMOOTH_LABEL_H

//#include "../widget/widget_label.h"
#include "colorable_label.h"
#include "brush/brush_solid.h"
#include "brush/brush_utils.h"

class SmoothLabel : public ColorableLabel
{
    typedef ColorableLabel base_t;

public:
    explicit SmoothLabel(Widget* parent, GCPoint const& pt, const char* text, GFXfont const& font = uistyle().inputFont())
    : base_t(parent, pt, text, font)
    {

    }

    explicit SmoothLabel(Widget* parent, GCPoint const& pt, Anchor a, const char* text, GFXfont const& font = uistyle().inputFont())
    : base_t(parent, pt, a, text, font)
    {

    }

    void onShow(bool s) override 
    {
        if (s) {
            base_t::color_norepaint(background().color(0,0)); // suppose background brush is solid
            m_animId = resetAnimation(m_animId, 0, 300, 0, 1);
        }
    }

    void onAnimation(int animId, float progress, int count) override 
    {
        const auto c1 = background().color(0,0); // suppose background brush is solid
        const auto c2 = m_colorTo;
        base_t::color(mix565colors(c1, c2, progress));
    }

    void color(uint16_t c)
    {
        m_colorTo = c;
    }

    uint16_t color() const
    {
        return m_colorTo;
    }

private:
    int m_animId = 0;
    uint16_t m_colorTo = Clr_GRAY;
};

#endif // file__SMOOTH_LABEL_H