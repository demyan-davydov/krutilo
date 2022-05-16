// -*- coding:utf-8-unix -*-
/*!
    @file       widget_header.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Page header widget
    @description

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#include "widget_header.h"
#include "../widget/ui_style.h"
#include "brush/brush_utils.h"


static constexpr auto k_bg = make_565_color(0, 100, 70);

WidgetHeader::WidgetHeader(Widget* parent, GCExtents2d const& ext, const char* text)
: Widget(parent, calcRect(text, ext, m_ext, Widget::measureContext()))
, m_text(text)
, m_background(make_565_color(0, 80, 30))
{
    
}

void WidgetHeader::text(const char* txt)
{
    m_text = txt;
}

void WidgetHeader::onAnimation(int animId, float progress, int count)
{
    m_progress = progress;
    sendRepaint();
}
    
void WidgetHeader::onAnimationFinished(int animId)
{
    if (m_animShowId == animId) {
        m_animShowId = 0;
    }
}

void WidgetHeader::onShow(bool s)
{
    m_animShowId = resetAnimation(m_animShowId, 0, 300, 0, 1);
    m_progress = 0;
}

void WidgetHeader::onPaint(GContext& ctx, GCRect const& rc)
{
    const auto c1 = parentBackground().color(0,0);
    constexpr auto c2 = k_bg;

    GCBrushSolid brushBg(mix565colors(c1, c2, m_progress));

    GCBrushSolid brushFg(Clr_BLACK);
    GFXfont const& f = uistyle().normalFont();

    ctx.drawTextSingle(m_text.c_str(), m_ext.topLeft(), f, brushFg, &brushBg);

    ctx.fillExtentsInner(rc, m_ext, brushBg);
}

GCRect WidgetHeader::calcRect(const char* text, GCExtents2d const& ext, GCExtents2d& ext_new, GContext const& ctx) // static
{
    auto sz = ctx.measureTextLine(uistyle().normalFont(), text);

    // cacl rigth extents
    auto const w = ctx.screenSize().sx;
    auto const r = ext.x.a + sz.sx;

    ext_new = ext;
    ext_new.x.b = r > w ? 0 : w - r;

    sz.sy += ext.y.sum();
    sz.sx = w;

    return GCRect::createSized({0,0}, sz);
}