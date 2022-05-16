// -*- coding:utf-8-unix -*-
/*!
    @file       widget_label.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Label widget
    @description
        Show text content

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__WIDGET_LABEL_H
#define file__WIDGET_LABEL_H

#include "widget.h"
#include "anchor.h"
#include "gc_extents2d.h"
#include "ui_style.h"
#include <string>

class WidgetLabel : public Widget
{
    typedef Widget base_t;

public:
    explicit WidgetLabel(Widget* parent, GCPoint const& pt, const char* text, GFXfont const& font = uistyle().inputFont());

    explicit WidgetLabel(Widget* parent, GCPoint const& pt, Anchor a, const char* text, GFXfont const& font = uistyle().inputFont());

    explicit WidgetLabel(Widget* parent, GCPoint const& pt, Anchor a, int em, GFXfont const& font = uistyle().inputFont());

    void contentAnchor(Anchor a);

    Anchor contentAnchor() const;

    void text(const char* txt, bool fitSize = false);

    void font(GFXfont* font, bool fitSize = false);

    void onPaint(GContext& ctx, GCRect const& rc) override;

    GCBrush const& background() const override;

    GCSize calcMinSize() const override;

    virtual GCBrush const& foreground() const;

    virtual GCExtents2d extents() const { return GCExtents2d{{0,0},{0,0}}; } 

    virtual void paintExtents(GContext&, GCRect const&) {}

    GFXfont const& font() const;

private:
    static GCRect calcRect(const char* text, GCPoint const& pt, Anchor a, GFXfont const& font, GContext const& ctx);

    static GCRect calcRect(int em, GCPoint const& pt, Anchor a, GFXfont const& font, GContext const& ctx);

    std::string m_text;
    GFXfont const* m_font;
    Anchor m_anchor = Anchor::TopLeft; // inner anchor
};

#endif // file__WIDGET_LABEL_H
