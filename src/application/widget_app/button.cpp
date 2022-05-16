// -*- coding:utf-8-unix -*em-
/*!
    @file       button.cpp
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-04

    @brief      Button like label
    @description
        Highlite then focused

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/


#include "button.h"
#include "anchor.h"
#include "../widget/ui_style.h"
#include "brush/brush_solid.h"
#include "brush/brush_radial_gradient.h"
#include "math_util.h"


int Button::calcWidthFor(const char* txt) // static
{
    auto const& ctx = base_t::measureContext();
    auto sz = ctx.measureTextLine(uistyle().buttonFont(), txt);
    sz.increaseBy(extents().toSize());
    return sz.sx;
}

GCRect Button::calcRect(std::string const& text, GCPoint const& pt, int minWidth, Anchor anc) // static
{
    auto const& ctx = base_t::measureContext();
    auto sz = ctx.measureTextLine(uistyle().buttonFont(), text.c_str(), text.size());
    sz.increaseBy( extents().toSize() );
    if (sz.sx < minWidth) {
        sz.sx = minWidth;
    }

    GCRect rc = createRelative(pt, sz, anc);

    return rc;
}

Button::Button(Widget* parent, GCPoint const& pt, std::string const& text, int minWidth, Anchor anchor) 
: base_t(parent, calcRect(text, pt, minWidth, anchor), Widget::State::k_default_tabstop)
, m_text(text)
{
    //m_state.tabStop(true);
    m_animGlowId = resetAnimation(m_animGlowId, 300, 2000, 2600);
}

void Button::contentAnchor(Anchor a)
{
    m_contentAnchor = a;
    sendRepaint();
}

Anchor Button::contentAnchor() const
{
    return m_contentAnchor;
}

void Button::text(std::string const& txt)
{
    m_text = txt;
    sendRepaint();

    // todo: fit to size?
}

void Button::drawButton(GContext& ctx, const GCRect& rc, GCBrush const& brushFg, GCBrush const& brushBg)
{
    GCRect rc_inner = rc.decreasedBy(extents());
    const int R = 2; // corner radius

    //auto r1 = rc.width();
    //GCBrushRadialGradient brush222(Clr_GREEN, Clr_YELLOW, rc.bottomCenter()-GCPoint_Y(r1), r1, r1+7);

    ctx.fillExtentsRound(rc_inner, R, extents(), brushBg);
    auto const& f = uistyle().buttonFont();
    ctx.drawTextSingle(m_text.c_str(), m_text.size(), rc_inner, contentAnchor(), {0,0}, f, brushFg, &brushBg);
}

void Button::onPaint(GContext& ctx, GCRect const&) 
{
    GCRect rc = rectLocal();
    uint16_t clrBg, clrFg;

    if (!isEnabled()) {
        clrBg = Clr_GRAY;
        clrFg = Clr_WHITE;
    }
    else {
        if (hasFocus()) {
            clrBg = Clr_DARK_GREEN;
            clrFg = make_565_color(0xff,0xff,0xa0);
            constexpr auto clrBg2 = make_565_color(0,200,0);

            float rOffset = rc.width()+300+m_glowRadius;
            float r1 = rOffset + m_glowAdd/2;
            float r2 = r1 + 15 + m_glowAdd;
            GCBrushRadialGradient bgBrush(clrBg2, clrBg, rc.bottomCenter()+GCPoint_Y(rOffset), r1, r2);
            GCBrushSolid textBrush(clrFg);
            drawButton(ctx, rc, textBrush, bgBrush);
            return;
        }
        else {
            clrBg = make_565_color(0,50,90);
            clrFg = make_565_color(0xb0,0xb0,0xb0);
        }
    }
    GCBrushSolid bgBrush(clrBg);
    GCBrushSolid textBrush(clrFg);

    drawButton(ctx, rc, textBrush, bgBrush);
}

void Button::onFocus(bool acquired) 
{
    /*
    if (acquired) {
        m_text = std::string("*") + m_text;
        postRepaint();
    }
    else {
        m_text = std::string("-") + m_text;
        postRepaint();
    }*/
}

void Button::onAnimation(int animId, float progress, int count)
{
    if (hasFocus() && animId == m_animGlowId) {
        const auto v = (1.0f - cosx(progress*my_PIx2*2))*(1.f-progress);
        m_glowRadius = v * 250;
        m_glowAdd = (v * 4); //v > 0 ? (v * 5 + 0.5) : (v * 5 - 0.5);
        sendRepaint();
    }
}

bool Button::onBubbling(MsgKeyUp const& m, Widget const* target)
{
    return true;
}

bool Button::onSinking(MsgKeyUp const&, Widget const* target) 
{
    return true;
}

bool Button::onBubbling(MsgKeyDown const&, Widget const* target)
{
    return true;
}

bool Button::onSinking(MsgKeyDown const&, Widget const* target)
{
    return true;
}

