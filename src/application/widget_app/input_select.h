// -*- coding:utf-8-unix -*-
/*!
    @file       input_select.h
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

#pragma once
#ifndef file__WIDGET_INPUT_SELECT_H
#define file__WIDGET_INPUT_SELECT_H

#include "../widget/widget.h"
#include "anim_helper_fade.h"
#include <vector>
#include <string>


class Select : public Widget
{
    static constexpr uint8_t k_arrow_spacer_width = 9;
    static constexpr uint8_t k_corner_radius = 3;

public:
    Select(Widget* parent, GContext const& ctx, GCPoint const& pt, int width);


    void onPaint(GContext& ctx, GCRect const& rc) override;

    void onAnimation(int animId, float progress, int count) override;

    void onAnimationFinished(int animId) override;

    void onShow(bool show) override;

    void onFocus(bool show) override;

    bool onBubbling(MsgKeyDown const&, Widget const* target) override;

    bool onBubbling(MsgKeyUp const&, Widget const* target) override;

    //bool tabStop() const override { return true; } // can be focused

    int selectedIndex() const { return m_sel; }

    /// криво косо, но все сроки уже ...
    /// по идее этот метод должен быть в отнаследованном классе, а этот класс должен быть абстрактным
    int rpm() const; 

    virtual int itemsCount() const;

    virtual const char* itemText(int idx) const;

    const char* currItemText() const;

private:
    bool hasLeftItems() const;

    bool hasRightItems() const;

    void moveLeft();

    void moveRight();

    static GCPoint drawStr(GContext& ctx, GCPoint const& pt, const char* txt, int posCursor);

    static GCSize calcMinSize(GContext const& ctx);

    static GCRect calcRect(GCCoord width, GCPoint const& pt, GContext const& ctx);

    static GCExtents2d extents() {return GCExtents2d{{4,4}, {5,3}}; }

    int m_sel = 0; ///< selected index

    AnimHelperFade m_helper_fade;
};

#endif // file__WIDGET_INPUT_SELECT_H
