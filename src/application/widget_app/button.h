// -*- coding:utf-8-unix -*em-
/*!
    @file       button.h
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

#pragma once
#ifndef file__BUTTON_H
#define file__BUTTON_H

#include "../widget/widget_label.h"
#include "anchor.h"
#include "gc_rect.h"
#include "gc_extents.h"
#include <string>

class Button : public Widget
{
    typedef Widget base_t;
    typedef Button self_t;

    std::string m_text;
    Anchor m_contentAnchor = Anchor::MidLeft;
    int m_animGlowId = 0;
    float m_glowRadius = 0;
    //int m_glowAdd = 0;
    float m_glowAdd = 0;

public:
    Button(Widget* parent, GCPoint const& pt, std::string const& text, int minWidth = 0, Anchor anchor = Anchor::TopLeft);

    void contentAnchor(Anchor a);

    Anchor contentAnchor() const;

    std::string const& text() const { return m_text; }

    void text(std::string const& txt);

    static int calcWidthFor(const char* txt);

protected:
    void drawButton(GContext& ctx, const GCRect& rc, GCBrush const& brushFg, GCBrush const& brushBg);

    void onPaint(GContext& ctx, GCRect const& rc) override;
    void onFocus(bool acquired) override;

    void onAnimation(int animId, float progress, int count) override;

    //bool tabStop() const override { return true; }
    bool onBubbling(MsgKeyUp const&, Widget const* target) override;
    bool onSinking(MsgKeyUp const&, Widget const* target) override;
    bool onBubbling(MsgKeyDown const&, Widget const* target) override;
    bool onSinking(MsgKeyDown const&, Widget const* target) override;

private:
    static GCRect calcRect(std::string const& text, GCPoint const& pt, int minWidth, Anchor anchor);
    static GCExtents2d extents() {return GCExtents2d{{8,8}, {2,5}}; }
};


#endif // file__BUTTON_H

