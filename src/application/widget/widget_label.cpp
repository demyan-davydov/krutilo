// -*- coding:utf-8-unix -*-
/*!
    @file       widget_label.cpp
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

#include "widget_label.h"
#include "ui_style.h"


WidgetLabel::WidgetLabel(Widget* parent, GCPoint const& pt, const char* text, GFXfont const& f)
: Widget(parent, calcRect(text, pt, Anchor::TopLeft, f, measureContext()))
, m_text(text)
, m_font(&f)
{
}

WidgetLabel::WidgetLabel(Widget* parent, GCPoint const& pt, Anchor a, const char* text, GFXfont const& f)
: Widget(parent, calcRect(text, pt, a, f, measureContext()))
, m_text(text)
, m_font(&f)
{
}

WidgetLabel::WidgetLabel(Widget* parent, GCPoint const& pt, Anchor a, int em, GFXfont const& f)
: Widget(parent, calcRect(em, pt, a, f, measureContext()))
, m_text()
, m_font(&f)
{
}

void WidgetLabel::contentAnchor(Anchor a)
{
    m_anchor = a;
}

Anchor WidgetLabel::contentAnchor() const
{
    return m_anchor;
}

void WidgetLabel::text(const char* txt, bool fitSize)
{
    if (0 != strcmp(m_text.c_str(), txt)) {
        m_text = txt;
        sendRepaint();
    }
}

void WidgetLabel::font(GFXfont* f, bool fitSize)
{
    m_font = f;

    if (fitSize) {
        base_t::setSize(calcMinSize());
    }
}

void WidgetLabel::onPaint(GContext& ctx, GCRect const&)
{
    const GCRect rc = rectLocal().decreasedBy(extents());

    ctx.drawTextSingle(m_text.c_str(), m_text.size(), rc, contentAnchor(), {0,0}, font(), foreground(), &background());
}

GCBrush const& WidgetLabel::background() const 
{
    return parentBackground();
}

GCSize WidgetLabel::calcMinSize() const
{
    auto const& ctx = measureContext();
    auto sz = ctx.measureTextLine(*m_font, m_text.c_str());
    return sz;
}

GCBrush const& WidgetLabel::foreground() const
{
    return uistyle().brushText();
}

GFXfont const& WidgetLabel::font() const
{
    return *m_font;
}

GCRect WidgetLabel::calcRect(const char* text, GCPoint const& pt, Anchor a, GFXfont const& font, GContext const& ctx) // static
{
    auto sz = ctx.measureTextLine(font, text);
    return createRelative(pt, sz, a);
}

GCRect WidgetLabel::calcRect(int em, GCPoint const& pt, Anchor a, GFXfont const& font, GContext const& ctx) // static
{
    return createRelative(pt, GCSize{font.emWidth()*em,font.yAdvance}, a);
}