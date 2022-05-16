// -*- coding:utf-8-unix -*-
/*!
    @file       colorable_label.h
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

#pragma once
#ifndef file__COLORABLE_LABEL_H
#define file__COLORABLE_LABEL_H

#include "../widget/widget_label.h"
#include "brush/brush_solid.h"

class ColorableLabel : public WidgetLabel
{
    typedef WidgetLabel base_t;

public:
    explicit ColorableLabel(Widget* parent, GCPoint const& pt, const char* text, GFXfont const& font = uistyle().inputFont());

    explicit ColorableLabel(Widget* parent, GCPoint const& pt, Anchor a, const char* text, GFXfont const& font = uistyle().inputFont());

    explicit ColorableLabel(Widget* parent, GCPoint const& pt, Anchor a, int em, GFXfont const& font = uistyle().inputFont());

    GCBrush const& foreground() const { return m_brushText; }

    void color_norepaint(uint16_t c);

    void color(uint16_t c);

    uint16_t color() const;

private:
    GCBrushSolid m_brushText;
};

#endif // file__COLORABLE_LABEL_H