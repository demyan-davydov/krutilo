// -*- coding:utf-8-unix -*-
/*!
    @file       ui_state.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-24

    @brief      UI widget state
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__UI_STATE_H
#define file__UI_STATE_H

enum class UIState
{
    Normal = 0,
    Focused = 1,
    Disabled = 2,
    Checked = 4,
    Pressed = 8,
    Error = 16,      /// wrong input
};

#endif // file__UI_STATE_H
