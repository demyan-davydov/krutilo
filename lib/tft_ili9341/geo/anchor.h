// -*- coding:utf-8-unix -*-
/*!
    @file       anchor.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-24

    @brief      Widget anchor for resize, move etc...
    @description
                one of point of the widget,
                relative to it widget will be moved/sized 
                Here is 9 points (digits placed like on keyboard keypad)
                7-8-9
                4-5-6
                1-2-3

                1 - means bottom-left corner
                4 - means middle-left point
                5 - means middle point of widget
                etc.

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__ANCHOR_H
#define file__ANCHOR_H

#include <cstdint> // uint8_t
#include "gc_rect.h"
#include "gc_size.h"

enum class Anchor : uint8_t
{
    TopLeft = 7, TopCenter = 8, TopRight = 9,
    MidLeft = 4, MidCenter = 5, MidRight = 6,
    BtmLeft = 1, BtmCenter = 2, BtmRight = 3, 
};

/// @brief resize oldRc to 'sz' relative to anchor
/// @return new rectangle resized relative to anchor
GCRect resize(GCRect const& oldRc, GCSize const& sz, Anchor a);

/// @brief create new rectangle relative to anchor 'a'
GCRect createRelative(GCPoint const& pt, GCSize const& sz, Anchor a);

/// @brief Place rectangle with size 'sz' inside of outer rectangle 'outRc'
/// @return topLeft point of new rectangle
GCPoint placeInside(GCRect const& outRc, GCSize const& sz, Anchor a);

/// @return corresponding point of rectangle
GCPoint pointOf(GCRect const& rc, Anchor a);

#endif // file__ANCHOR_H