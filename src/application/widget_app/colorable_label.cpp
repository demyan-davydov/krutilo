// -*- coding:utf-8-unix -*-
/*!
    @file       colorable_label.cpp
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-28

    @brief      label with text color
    @description
        Show text content

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/


#include "colorable_label.h"

ColorableLabel::ColorableLabel(Widget* parent, GCPoint const& pt, const char* text, GFXfont const& font)
: base_t(parent, pt, text, font)
, m_brushText(Clr_BLACK)
{

}

ColorableLabel::ColorableLabel(Widget* parent, GCPoint const& pt, Anchor a, const char* text, GFXfont const& font)
: base_t(parent, pt, a, text, font)
, m_brushText(Clr_BLACK)
{

}

ColorableLabel::ColorableLabel(Widget* parent, GCPoint const& pt, Anchor a, int em, GFXfont const& font)
: base_t(parent, pt, a, em, font)
, m_brushText(Clr_BLACK)
{

}

void ColorableLabel::color_norepaint(uint16_t c) {
    m_brushText.color(c);
}

void ColorableLabel::color(uint16_t c) {
    m_brushText.color(c);
    sendRepaint();
}

uint16_t ColorableLabel::color() const {
    return m_brushText.color();
}
