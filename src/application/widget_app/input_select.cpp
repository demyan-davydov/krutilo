// -*- coding:utf-8-unix -*-
/*!
    @file       input_select.cpp
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-16

    @brief      Select one value of presented
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#include "input_select.h"
#include "labels_win1251.h"
#include "../widget/ui_style.h"
#include "../application_keys.h"


Select::Select(Widget* parent, GContext const& ctx, GCPoint const& pt, int width)
: Widget(parent, calcRect(width, pt, ctx), Widget::State::k_default_tabstop)
{

}

static void drawArrow(GContext& ctx, GCBrush const& brushFg, GCBrush const& brushBg, GCRect const& rc, int width, bool left) 
{
    constexpr int gap = 0;
    constexpr int arrow_w = 5;
    constexpr int arrow_h = (arrow_w - 1) * 2 + 1;

    const int k = left ? 1 : -1;
    const int y = rc.ry.center();
    const int x = left ? rc.rx.a + gap : rc.rx.b - gap;

    const auto top = y - arrow_w + 1;
    auto xx = x;
    for (int i = 0; i < arrow_w; ++i, xx += k) {
        const auto h_blank = arrow_w - i - 1;
        const auto h_fill = i*2 + 1;
        ctx.drawVLine({xx, top}, h_blank, brushBg);
        ctx.drawVLine({xx, y-i}, h_fill, brushFg);
        ctx.drawVLine({xx, y+i+1}, h_blank, brushBg);
    }

    const auto pt = left ? rc.topLeft() : rc.topRight() - GCPoint_X(width-1);

    // rect includes arrow
    const auto ay1 = y - arrow_w + 1;
    const auto ay2 = y + arrow_w - 1;
    const auto ax1 = left ? x : x - arrow_w + 1;
    const auto ax2 = left ? x + arrow_w - 1 : x;

    // fill top
    ctx.fillRect(GCRect::createSized(pt, {width, ay1 - rc.ry.a}), brushBg);

    // fill bottom
    ctx.fillRect(GCRect::createSized(GCPoint(pt.x,ay2+1), {width, rc.ry.b-ay2}), brushBg);

    // fill left subrect
    ctx.fillRect(GCRect::createSized(GCPoint(pt.x, ay1), {ax1-pt.x, arrow_h}), brushBg);

    // fill rigth subrect
    ctx.fillRect(GCRect::createSized(GCPoint(ax2+1, ay1), {width-arrow_w-(ax1-pt.x), arrow_h}), brushBg);
}

static void drawCentered(GContext& ctx, const char* txt, GFXfont const& f, GCBrush const& fg,  GCBrush const& bg, int width, GCPoint& pt)
{
    auto sz = ctx.measureTextLine(f, txt, strlen(txt));
    const auto left_offset = (width - sz.sx) / 2;
    GCPoint p = pt; p.x += left_offset;

    ctx.drawTextSingle(txt, p, f, fg, &bg);

    // fill left and rigth areas
    {
        ctx.fillRect(GCRect::createSized(pt, {p.x-pt.x, sz.sy}), bg); // left
        ctx.fillRect(GCRect::createSized(p + GCPoint_X(sz.sx), {width-left_offset-sz.sx, sz.sy}), bg); // right
    }

    pt.y += f.yAdvance;
}

void Select::onPaint(GContext& ctx, GCRect const& /*rcPaint*/)
{
    const char* text_main = currItemText();// "60";
    const char* text_hint = label_rpm_short;// "rpm";

    auto const& f1 = uistyle().bigNumericFont(); // font for main text ("60")
    auto const& f2 = uistyle().smallFont();      // font for subscript ("rpm")

    GCBrushSolid brushBg(m_helper_fade.bgColor(*this));
    GCBrushSolid brushFgMain(Clr_BLACK);  // color of main text ("60")
    GCBrushSolid brushFgHint(Clr_GRAY);   // color of subscript ("rpm")

    GCRect rc_inner = rectLocal().decreasedBy(extents());

    ctx.fillExtentsRound(rc_inner, k_corner_radius, extents(), brushBg);

    drawArrow(ctx, hasLeftItems()  ? brushFgMain : brushFgHint, brushBg, rc_inner, k_arrow_spacer_width, true);
    drawArrow(ctx, hasRightItems() ? brushFgMain : brushFgHint, brushBg, rc_inner, k_arrow_spacer_width, false);

    rc_inner.decreaseBy(GCExtents2d{{k_arrow_spacer_width,k_arrow_spacer_width},{0,0}});
    GCPoint cur = rc_inner.topLeft();
    const int w = rc_inner.width();

    drawCentered(ctx, text_main, f1, brushFgMain, brushBg, w, cur);
    drawCentered(ctx, text_hint, f2, brushFgHint, brushBg, w, cur);
}

void Select::onAnimation(int animId, float p, int count) 
{
    m_helper_fade.onAnimation(animId, p);
    sendRepaint();
}

void Select::onAnimationFinished(int animId)
{
    m_helper_fade.onAnimationFinished(animId);
}

void Select::onShow(bool show)
{
    m_helper_fade.onShow(*this, show);
}

void Select::onFocus(bool f)
{
    m_helper_fade.onFocus(*this, f);
}

bool Select::onBubbling(MsgKeyDown const& m, Widget const* target)
{
    return true;
}

bool Select::onBubbling(MsgKeyUp const& m, Widget const* target)
{
    if (target == this)
    {
        if (m.key == VK_LEFT) {
            moveLeft();
        }
        else if (m.key == VK_RIGHT) {
            moveRight();
        }

        postRepaint();
    }

    return true;
}

int Select::rpm() const
{
    switch(selectedIndex()) {
        case 0: return 60;
        case 1: return 72;
        case 2: return 200;
        case 3: return 400;
        case 4: return 600;
        case 5: return 800;
        case 6: return 1000;
        //case 2: return 720;
    }
    return 0;
}

int Select::itemsCount() const
{
    return 7;
}

const char* Select::itemText(int idx) const
{
    int v = rpm();

    // stupid itoa:
    constexpr int buflen = 8; // maximum 5 digits, but max RPM now is 720, so it must be enought
    static char buf[buflen];
    buf[buflen-1] = 0;
    char* p = &buf[buflen-2];
    //*p = '0';

    do {
        *p-- = '0' + (v % 10); 
        v /= 10;
    } while (v);

    return p+1;
}

const char* Select::currItemText() const
{
    if (m_sel >= 0) {
        return itemText(m_sel);
    }
    return "0";
}

bool Select::hasLeftItems() const
{
    return itemsCount() && m_sel > 0;
}

bool Select::hasRightItems() const
{
    return m_sel + 1 < itemsCount();
}

void Select::moveLeft()
{
    if (m_sel > 0) {
        --m_sel;
    }
}

void Select::moveRight()
{
    if (m_sel + 1 < itemsCount()) {
        ++m_sel;
    }
}

GCSize Select::calcMinSize(GContext const& ctx) // static
{
    auto const& f1 = uistyle().bigNumericFont();
    auto const& f2 = uistyle().smallFont();

    // suppose each digit takes same size
    const GCSize digitSize1 = ctx.measureTextLine(f1, "000");
    GCSize fullSize = digitSize1;
    fullSize.sy += f2.yAdvance;
    fullSize.sx += + k_arrow_spacer_width * 2;

    return fullSize;
}

GCRect Select::calcRect(GCCoord width, GCPoint const& pt, GContext const& ctx) // static
{
    auto sz = calcMinSize(ctx) + extents().toSize();

    sz.sx = std::max(sz.sx, width);
    return GCRect::createSized(pt, sz);
}

