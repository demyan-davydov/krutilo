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

#include "input_time.h"
#include "../widget/ui_style.h"
#include "../application_keys.h"


InputTime::InputTime(Widget* parent, GContext const& ctx, GCPoint const& pt, int width)
: Widget(parent, calcRect(width, pt, ctx), Widget::State::k_default_tabstop)
{

}

static void drawChar(GContext& ctx, char c, GFXfont const& font, GCBrush const& fg, GCBrush const& bg, GCPoint& pt)
{
    pt.x -= (int)ctx.measureCharWidth(font, c);
    ctx.drawChar(c, pt, font, fg, &bg);
}

static void drawStr(GContext& ctx, const char* s, GFXfont const& font, GCBrush const& fg, GCBrush const& bg, GCPoint& pt)
{
    auto last = s + strlen(s) - 1;
    while (last >= s) {
        drawChar(ctx, *last--, font, fg, bg, pt);
    }
}

static void drawGap(GContext& ctx, uint8_t gap, uint8_t height, GCBrush const& bg, GCPoint& pt)
{
    pt.x -= gap;
    ctx.fillRect(GCRect::createSized(pt, {gap, height}), bg);
}

static char popChar(uint16_t& v)
{
    const char c = '0' + v % 10;
    v /= 10;
    return c;
}

void InputTime::onPaint(GContext& ctx, GCRect const& rcPaint)
{
    GCRect rc = rectLocal();

    GCBrushSolid brushBg(m_animHelper.bgColor(*this));

    GCBrushSolid brushBgSel(Clr_LIGHT_GRAY); // color of selected item
    GCBrushSolid brushBgPressed(Clr_YELLOW); // background color for pressed char
    
    GCBrushSolid brushFg(Clr_BLACK); // text color
    GCBrushSolid brushFgText(Clr_GRAY); // color of marks

    auto const& f = uistyle().inputFont();
    const bool focused = hasFocus();

    // draw frame, left part, N digits of hours, space, 
    // mark hours, space, M digits of minutes

    const GCRect rc_inner = rc.decreasedBy(extents());
    const auto rc_inner_h = rc_inner.height();
    ctx.fillExtentsRound(rc_inner, k_corner_radius, extents(), brushBg);

    GCPoint cur = rc_inner.topRight(); cur.x += 1;

    // minutes text
    drawStr(ctx, k_str_minutes, f, brushFgText, brushBg, cur);

    // right gap
    drawGap(ctx, k_gap, rc_inner_h, brushBg, cur);

    uint8_t digit_pos = 0;

    // draw minutes
    {
        uint16_t v = m_minutes;
        for (uint8_t i = 0; i < k_minutes_digits; ++i, ++digit_pos) {
            drawChar(ctx, popChar(v), f, brushFg, 
                focused && digit_pos == m_pos ? (m_pressed ? brushBgPressed : brushBgSel) : brushBg, cur);
        }
    }

    drawGap(ctx, k_gap, rc_inner_h, brushBg, cur);
    drawStr(ctx, k_str_hours, f, brushFgText, brushBg, cur);
    drawGap(ctx, k_gap, rc_inner_h, brushBg, cur);

    // draw hours
    {
        uint16_t v = m_hours;
        for (uint8_t i = 0; i < k_hours_digits; ++i, ++digit_pos) {
            drawChar(ctx, popChar(v), f, brushFg, 
                focused && digit_pos == m_pos ? (m_pressed ? brushBgPressed : brushBgSel) : brushBg, cur);
        }
    }

    // draw least rect
    {
        GCRect rc_rigth = rc_inner;
        rc_rigth.rx.b = cur.x-1;
        ctx.fillRect(rc_rigth, brushBg);
    }

    if (focused && (m_animHelper.blink() || m_pressed)) {
        ctx.drawRect(cursorRect(ctx), brushFg);
    }
}

void InputTime::onAnimation(int animId, float p, int count) 
{
    m_animHelper.onAnimation(animId, p);
    sendRepaint(); 
}

void InputTime::onAnimationFinished(int animId)
{
    m_animHelper.onAnimationFinished(animId);
}

void InputTime::onShow(bool show)
{
    m_animHelper.onShow(*this, show);
}

void InputTime::onFocus(bool f)
{
    m_animHelper.onFocus(*this, f);
}

bool InputTime::onBubbling(MsgKeyDown const& m, Widget const* target)
{
    if (target == this)
    {
        if (isInputChar(m.key)) {
            m_pressed = true;
            replaceCurrDigit(m.key - '0');
        }

        sendRepaint();
    }

    return true;
}

bool InputTime::onBubbling(MsgKeyUp const& m, Widget const* target)
{
    if (target == this)
    {
        m_pressed = false;
        if (isInputChar(m.key)) {
            if (replaceCurrDigit(m.key - '0'))
                moveCursorRight();
        }
        else if (m.key == VK_LEFT) {
            moveCursorLeft();
        }
        else if (m.key == VK_RIGHT) {
            moveCursorRight();
        }

        sendRepaint();
    }

    return true;
}

uint32_t InputTime::totalMinutes() const
{
    return m_hours * 60 + m_minutes;
}

uint32_t InputTime::totalSeconds() const
{
    return totalMinutes() * 60;
}

GCRect InputTime::cursorRect(GContext const& ctx) const
{
    auto const& f = uistyle().inputFont();

    // suppose each digit takes same size
    const int digitW = ctx.measureCharWidth(f, '0');
    const int t1W = ctx.measureTextLine(f, k_str_hours).sx;
    const int t2W = ctx.measureTextLine(f, k_str_minutes).sx;
    GCPoint pt = rectLocal().bottomRight() - GCPoint(extents().x.b+t2W+k_gap-1, extents().y.b);

    if (m_pos >= k_minutes_digits) {
        pt.x -= t1W;
        pt.x -= k_gap*2;
    }

    pt.x -= digitW * (m_pos+1);

    return GCRect::createSized(pt, GCSize(digitW, 2));
}

void InputTime::drawCursor(GContext& ctx)
{
    GCBrushSolid brush(Clr_RED);
    ctx.drawRect(cursorRect(ctx), brush);    
}

static uint16_t replaceNthDigit(uint16_t v, uint16_t newDigit, uint8_t n) 
{
    uint16_t curr = v;
    uint16_t k = 1;
    while (n--){ curr /= 10; k *= 10; }
    auto prevDigit = curr % 10;

    return (v - prevDigit * k) + newDigit * k;
}

bool InputTime::replaceCurrDigit(uint8_t d)
{
    if (m_pos < k_minutes_digits) {
        auto v = replaceNthDigit(m_minutes, d, m_pos);
        if (v > 59) 
            return false;
        m_minutes = v;
    }
    else {
        m_hours = replaceNthDigit(m_hours, d, m_pos-k_minutes_digits);
    }

    return true;
}

void InputTime::moveCursorLeft()
{
    if (m_pos + 1 < k_total_digits) 
        ++m_pos;
}

void InputTime::moveCursorRight()
{
    if (m_pos > 0) 
        --m_pos;
}

bool InputTime::isInputChar(char c) // static
{
    return  (c >= '0' && c <= '9');
}

GCSize InputTime::calcMinSize(GContext const& ctx) // static
{
    auto const& f = uistyle().inputFont();

    // suppose each digit takes same size
    const GCSize digitSize = ctx.measureChar(f, '0');
    const GCSize t1Size = ctx.measureTextLine(f, k_str_hours);
    const GCSize t2Size = ctx.measureTextLine(f, k_str_minutes);
    GCSize fullSize = digitSize;
    fullSize.sx *= k_total_digits;
    fullSize.sx += k_gap * 3; 
    fullSize.sx += t1Size.sx + t2Size.sx;

    return fullSize;
}

GCRect InputTime::calcRect(GCCoord width, GCPoint const& pt, GContext const& ctx) // static
{
    auto sz = calcMinSize(ctx) + extents().toSize();
    sz.sx = std::max(sz.sx, width);
    return GCRect::createSized(pt, sz);
}

