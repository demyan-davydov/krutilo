// -*- coding:utf-8-unix -*-
/*!
    @file       ui_style.cpp
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

#include "ui_style.h"

#include "brush_solid.h"
#include "brush_strip.h"
#include "brush_radial_gradient.h"
#include "brush_radial_gradient.h"
#include "brush_utils.h"

#include "surface_ili9341.h"

#include "fonts/PT_Mono_9.h"
#include "fonts/PT_Mono_10.h"
#include "fonts/PT_Mono_13.h"
#include "fonts/PT_Mono_15_dig.h"

UIStyle::UIStyle()
: m_brushBackground(Clr_NAVY)
, m_brushText(Clr_BLACK)
, m_brushInputBg(Clr_WHITE)
, m_brushInputText(Clr_BLACK)
{
}

uint16_t UIStyle::getInputBgColor(bool focused, float fadeProgress)
{
    const uint16_t bgColor0 = focused ? Clr_WHITE : Clr_LIGHT_YELLOW;
    const uint16_t bgColor1 = focused ? Clr_LIGHT_YELLOW : Clr_WHITE;
    return fadeProgress == 1 ? bgColor1 : mix565colors(bgColor0, bgColor1, fadeProgress);
}

uint16_t UIStyle::getInputBgColor(bool focused)
{
    return focused ? Clr_LIGHT_YELLOW : Clr_WHITE;
}

GFXfont const &UIStyle::normalFont() const
{
    return PTMono_13pt;
}

GFXfont const &UIStyle::bigNumericFont() const
{
    return PTMono_15pt;
}

GFXfont const &UIStyle::smallFont() const
{
    return PTMono_9pt;
}

GCBrush const &UIStyle::brushBackground() const
{
    return m_brushBackground;
}

GCBrush const &UIStyle::brushText() const
{
    return m_brushText;
}

GFXfont const &UIStyle::buttonFont() const
{
    return normalFont();
}

GFXfont const &UIStyle::inputFont() const
{
    return normalFont();
}

GCBrush const &UIStyle::inputBrushBg(int state) const
{
    return m_brushInputBg;
}

GCBrush const &UIStyle::inputBrushText(int state) const
{
    return m_brushInputText;
}