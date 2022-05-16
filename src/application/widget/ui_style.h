// -*- coding:utf-8-unix -*-
/*!
    @file       ui_style.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-24

    @brief      UU style presets
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__UI_STYLE_H
#define file__UI_STYLE_H

#include "brush_solid.h"
#include "brush_strip.h"
//#include "brush_radial_gradient.h"
//#include "brush_radial_gradient.h"
//#include "gc_all.h"

#include "gfxfont.h"
#include "ui_state.h"

class UIStyle
{
    UIStyle();
    UIStyle(UIStyle const &) = delete;
    UIStyle &operator=(UIStyle const &) = delete;

public:
    static UIStyle &instance()
    {
        // согласно стандарту, этот код ленивый и потокобезопасный
        static UIStyle s;
        return s;
    }

    static uint16_t getInputBgColor(bool focused, float fadeProgress);
    static uint16_t getInputBgColor(bool focused);

    GFXfont const &normalFont() const;
    GFXfont const &bigNumericFont() const;
    GFXfont const &smallFont() const;

    /// default background brush
    GCBrush const &brushBackground() const;

    /// default text brush
    GCBrush const &brushText() const;

    /// default button font
    GFXfont const &buttonFont() const;

    GFXfont const &inputFont() const;
    GCBrush const &inputBrushBg(int state) const;
    GCBrush const &inputBrushText(int state) const;

private:
    GCBrushSolid m_brushBackground;
    GCBrushSolid m_brushText;
    GCBrushSolid m_brushInputBg;
    GCBrushSolid m_brushInputText;
};

inline UIStyle const &uistyle() { return UIStyle::instance(); }

#endif // file__UI_STYLE_H
