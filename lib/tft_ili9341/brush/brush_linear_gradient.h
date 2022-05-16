#pragma once
#ifndef file__BRUSH_LINEAR_GRADIENT_H
#define file__BRUSH_LINEAR_GRADIENT_H

#include "brush.h"
#include "brush_utils.h"
#include "geo/gc_point.h"

class GCBrushLinearGradient : public GCBrush
{
    uint16_t c1, c2;
    GCPoint pt1, pt2;

public:
    GCBrushLinearGradient(uint16_t c1_, uint16_t c2_, GCPoint const& pt1_, GCPoint const& pt2_) 
    : c1(c1_), c2(c2_), pt1(pt1_), pt2(pt2_) {}
    
    uint16_t color(int x, int y) const override
    {
        GCPoint pt(x,y);
        const uint16_t gap = pt1.squaredDistanceTo(pt2);
        const uint16_t d1 = pt1.squaredDistanceTo(pt);
        const uint16_t d2 = pt2.squaredDistanceTo(pt);

        if (d1 > gap) { // point behind d2
            return c2;
        }
        else if (d2 > gap) { // point behind d1
            return c1;
        }

        return mix565colors(c1, c2, d2, d1);
    }

    virtual GCBrush* clone() const { return new GCBrushLinearGradient(*this); }

    void paint(GCRect const& rc, Device_ILI9341& device) const override
    {
        for (int y = rc.y1(); y <= rc.y2(); ++y)
        for (int x = rc.x1(); x <= rc.x2(); ++x)
        {
            device.fastWrite(GCBrushLinearGradient::color(x,y));
        }
    };

    void paint(GCPoint const& pt, Device_ILI9341& device) const override
    {
        device.fastWrite(GCBrushLinearGradient::color(pt.x,pt.y));
    };
};

#endif // file__BRUSH_LINEAR_GRADIENT_H
