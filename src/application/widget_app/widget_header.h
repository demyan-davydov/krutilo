// -*- coding:utf-8-unix -*-
/*!
    @file       widget_header.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-16

    @brief      Page Header widget
    @description
        
    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__WIDGET_HEADER_H
#define file__WIDGET_HEADER_H

#include "../widget/widget.h"
#include "brush_solid.h"
#include <string>

class WidgetHeader : public Widget
{
    static constexpr int k_offset_x = 15;
    static constexpr int k_offset_y = 12;
    static constexpr int k_gap_y = 4;
    static constexpr int k_line_h = 2;

public:
    WidgetHeader(Widget* parent, GCExtents2d const& ext, const char* txt);


    void text(const char* txt);

    std::string const& text() const { return m_text; }

    void onPaint(GContext& ctx, GCRect const& rc) override;

    void onAnimation(int animId, float progress, int count) override;
     
    void onAnimationFinished(int animId) override;

    void onShow(bool s) override;

    GCBrush const& background() const override { return m_background; }

private:
    static GCRect calcRect(const char* text, GCExtents2d const& ext, GCExtents2d& ext_new, GContext const& ctx);

    std::string m_text;

    GCExtents2d m_ext;

    GCBrushSolid m_background;

    int m_animShowId = 0;

    float m_progress = 0; // animation progress
};

#endif // file__WIDGET_HEADER_H
