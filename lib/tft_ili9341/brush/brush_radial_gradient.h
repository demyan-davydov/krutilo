#pragma once
#ifndef file__BRUSH_RADIAL_GRADIENT_H
#define file__BRUSH_RADIAL_GRADIENT_H

#include "brush.h"
#include "brush_utils.h"
#include "geo/gc_point.h"

class GCBrushRadialGradient : public GCBrush
{
    uint16_t c1; // color at center
    uint16_t c2; // color behind radius
    uint32_t ra2, rb2;
    GCPoint center;

public:
    GCBrushRadialGradient(uint16_t color1, uint16_t color2, GCPoint const& ptCenter, float radius1, float radius2) 
    : c1(color1), c2(color2), ra2(radius1*radius1*16), rb2(radius2*radius2*16), center(ptCenter) {}

    uint16_t color(int x, int y) const override
    {
        GCPoint pt(x,y);
        const uint32_t d = pt.squaredDistanceTo(center) << 4;

        if (d <= ra2) {
            return c1;
        }
        if (d >= rb2) { // point behind d2
            return c2;
        }

        return mix565colors_fix4(c1, c2, rb2-d, d-ra2);
    }

    virtual GCBrush* clone() const { return new GCBrushRadialGradient(*this); }

    void paint(GCRect const& rc, Device_ILI9341& device) const override
    {
        for (int y = rc.y1(); y <= rc.y2(); ++y)
        for (int x = rc.x1(); x <= rc.x2(); ++x)
        {
            device.fastWrite(GCBrushRadialGradient::color(x,y));
        }
    };

    void paint(GCPoint const& pt, Device_ILI9341& device) const override
    {
        device.fastWrite(GCBrushRadialGradient::color(pt.x,pt.y));
    };
};

#endif // file__BRUSH_RADIAL_GRADIENT_H
