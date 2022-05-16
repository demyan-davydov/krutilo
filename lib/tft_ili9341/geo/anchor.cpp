// -*- coding:utf-8-unix -*-
/*!
    @file       anchor.cpp
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

#include "anchor.h"


GCRect resize(GCRect const& oldRc, GCSize const& sz, Anchor a)
{
    //const prevSz;

    switch (a)
    {
    case Anchor::TopLeft:       return GCRect::createSized(oldRc.topLeft(), sz);
    case Anchor::TopCenter:     return GCRect::createSized(oldRc.topCenter(), sz);
    case Anchor::TopRight:      return GCRect::createSized(oldRc.topRight(), sz);
    case Anchor::MidLeft:       return GCRect::createSized(oldRc.midLeft(), sz);
    case Anchor::MidCenter:     return GCRect::createSized(oldRc.midCenter(), sz);
    case Anchor::MidRight:      return GCRect::createSized(oldRc.midRight(), sz);
    case Anchor::BtmLeft:       return GCRect::createSized(oldRc.bottomLeft(), sz);
    case Anchor::BtmCenter:     return GCRect::createSized(oldRc.bottomCenter(), sz);
    case Anchor::BtmRight:      return GCRect::createSized(oldRc.bottomRight(), sz);
    }

    return oldRc;
}

GCRect createRelative(GCPoint const& pt, GCSize const& sz, Anchor a)
{
    //const prevSz;

    switch (a)
    {
    case Anchor::TopLeft:       return GCRect::createSized(pt, sz);
    case Anchor::TopCenter:     return GCRect::createSized(GCPoint(pt.x-sz.sx/2, pt.y), sz);
    case Anchor::TopRight:      return GCRect::createSized(GCPoint(pt.x-sz.sx,   pt.y), sz);

    case Anchor::MidLeft:       return GCRect::createSized(GCPoint(pt.x,         pt.y-sz.sy/2), sz);
    case Anchor::MidCenter:     return GCRect::createSized(GCPoint(pt.x-sz.sx/2, pt.y-sz.sy/2), sz);
    case Anchor::MidRight:      return GCRect::createSized(GCPoint(pt.x+sz.sx,   pt.y-sz.sy/2), sz);
    
    case Anchor::BtmLeft:       return GCRect::createSized(GCPoint(pt.x,         pt.y-sz.sy), sz);
    case Anchor::BtmCenter:     return GCRect::createSized(GCPoint(pt.x-sz.sx/2, pt.y-sz.sy), sz);
    case Anchor::BtmRight:      return GCRect::createSized(GCPoint(pt.x-sz.sx,   pt.y-sz.sy), sz);
    }

    return GCRect::createSized(pt, sz);
}

GCPoint placeInside(GCRect const& outRc, GCSize const& sz, Anchor a)
{
    switch (a)
    {
    case Anchor::TopLeft:       return outRc.topLeft();
    case Anchor::TopCenter:     return outRc.topCenter()    - GCPoint_X(sz.sx/2);
    case Anchor::TopRight:      return outRc.topRight()     - GCPoint_X(sz.sx);

    case Anchor::MidLeft:       return outRc.midLeft()      - GCPoint_Y(sz.sy/2);
    case Anchor::MidCenter:     return outRc.midCenter()    - GCPoint(sz.sx/2, sz.sy/2);
    case Anchor::MidRight:      return outRc.midRight()     - GCPoint(sz.sx, sz.sy/2);
    
    case Anchor::BtmLeft:       return outRc.bottomLeft()   - GCPoint_Y(sz.sy);
    case Anchor::BtmCenter:     return outRc.bottomCenter() - GCPoint(sz.sx/2, sz.sy);
    case Anchor::BtmRight:      return outRc.bottomRight()  - GCPoint(sz.sx, sz.sy);
    }

    return outRc.topLeft();
}

GCPoint pointOf(GCRect const& rc, Anchor a)
{
    switch (a)
    {
    case Anchor::TopLeft:       return rc.topLeft();
    case Anchor::TopCenter:     return rc.topCenter();
    case Anchor::TopRight:      return rc.topRight();

    case Anchor::MidLeft:       return rc.midLeft();
    case Anchor::MidCenter:     return rc.midCenter();
    case Anchor::MidRight:      return rc.midRight();
    
    case Anchor::BtmLeft:       return rc.bottomLeft();
    case Anchor::BtmCenter:     return rc.bottomCenter();
    case Anchor::BtmRight:      return rc.bottomRight();
    }

    return rc.topLeft();
}

