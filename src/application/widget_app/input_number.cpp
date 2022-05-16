// -*- coding:utf-8-unix -*-
/*!
    @file       input_number.cpp
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

#include "input_number.h"
#include "../widget/ui_style.h"
#include "../application_keys.h"
#include "../tft_ili9341/brush/brush_utils.h" // mix colors


InputNumber::InputNumber(Widget* parent, GContext const& ctx, GCPoint const& pt, int width)
: Widget(parent, calcRect(width, pt, ctx), Widget::State::k_default_tabstop )
{

}

static char popChar(uint32_t& v)
{
    const char c = '0' + v % 10;
    v /= 10;
    return c;
}

static void drawChar(GContext& ctx, char c, GFXfont const& font, GCBrush const& fg, GCBrush const& bg, GCPoint& pt)
{
    pt.x -= ctx.measureCharWidth(font, c);
    ctx.drawChar(c, pt, font, fg, &bg);
}

void InputNumber::onPaint(GContext& ctx, GCRect const& rcPaint)
{
    GCRect rc = rectLocal();

    GCBrushSolid brushBg(m_animHelper.bgColor(*this));
    GCBrushSolid brushFg(Clr_BLACK);
    GCBrushSolid& brushCursor = brushFg;
    GCBrushSolid brushBgPressed(m_animHelper.pressedBg()); // color of marks
    GCBrushSolid brushBgErased(Clr_PINK); // color of marks
    
    auto const& f = uistyle().inputFont();

    const GCRect rc_inner = rc.decreasedBy(extents());
    ctx.fillExtentsRound(rc_inner, k_corner_radius, extents(), brushBg);

    GCPoint cur = rc_inner.topRight(); cur.x += 1;

    // draw cursor
    {
        cur.x -= k_cursor_w;
        auto rc_cursor = cursorRect(ctx);
        ctx.fillRect(rc_cursor, hasFocus() && (m_animHelper.blink() || m_pressed) ? brushCursor : brushBg);
    }

    // draw gap
    {
        cur.x -= k_gap;
        ctx.fillRect(GCRect::createSized(cur, {k_gap, rc_inner.height()}), brushBg);
    }

    auto v = m_value;

    // draw first digit 
    drawChar(ctx, popChar(v), f, brushFg, 
        m_pressed ? (m_pressed_char == VK_LEFT ? brushBgErased : brushBgPressed) : brushBg, cur );
    
    // draw least digits 
    while (v) {
        drawChar(ctx, popChar(v), f, brushFg, brushBg, cur );
    }
    
    // draw least rect
    {
        GCRect rc_rigth = rc_inner;
        rc_rigth.rx.b = cur.x - 1;
        ctx.fillRect(rc_rigth, brushBg);
    }
}

void InputNumber::onAnimation(int animId, float p, int count) 
{
    m_animHelper.onAnimation(animId, p);
    sendRepaint();
}

void InputNumber::onAnimationFinished(int animId)
{
    m_animHelper.onAnimationFinished(animId);
}

void InputNumber::onShow(bool show)
{
    m_animHelper.onShow(*this, show);
}

void InputNumber::onFocus(bool f)
{
    m_animHelper.onFocus(*this, f);
}

bool InputNumber::onBubbling(MsgKeyDown const& m, Widget const* target)
{
    if (target == this)
    {
        m_pressed_char = m.key;

        if (isInputChar(m.key) && digitsCount() < k_max_digits ) {
            m_pressed = true;
            appendDigit(m.key - '0');
        }
        else if (m.key == VK_LEFT) {
            m_pressed = true;
        }

        sendRepaint();
    }

    return true;
}

bool InputNumber::onBubbling(MsgKeyUp const& m, Widget const* target)
{
    if (target == this)
    {
        m_pressed = false;
        if (m.key == VK_LEFT) {
            eraseLastDigit();
        }
        sendRepaint();
    }

    return true;
}

uint32_t InputNumber::value() const
{
    return m_value;
}

GCRect InputNumber::cursorRect(GContext& ctx) const
{
    auto rc = rectLocal().decreasedBy(extents());
    rc.rx.resizeFromB(k_cursor_w);
    return rc;
}

void InputNumber::drawCursor(GContext& ctx)
{
    GCBrushSolid brush(Clr_RED);
    ctx.drawRect(cursorRect(ctx), brush);    
}

void InputNumber::appendDigit(char v)
{
    m_value *= 10;
    m_value += v;
}

void InputNumber::eraseLastDigit()
{
    m_value /= 10;
}

uint8_t InputNumber::digitsCount() const
{
    uint32_t v = m_value;
    uint8_t n = 1;
    for(; v; v /= 10, ++n) {}
    return n;
}

bool InputNumber::isInputChar(char c) // static
{
    return  (c >= '0' && c <= '9');
}

GCSize InputNumber::calcMinSize(GContext const& ctx) // static
{
    auto const& f = uistyle().inputFont();

    // suppose each digit takes same size
    const GCSize digitSize = ctx.measureChar(f, '0');
    GCSize fullSize = digitSize;
    fullSize.sx *= k_max_digits;
    fullSize.sx += k_cursor_w;
    fullSize.sx += k_gap;

    return fullSize;
}

GCRect InputNumber::calcRect(GCCoord width, GCPoint const& pt, GContext const& ctx) // static
{
    auto sz = calcMinSize(ctx) + extents().toSize();
    sz.sx = std::max(sz.sx, width);
    return GCRect::createSized(pt, sz);
}

