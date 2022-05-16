#pragma once
#ifndef file__BRUSH_STRIP_H
#define file__BRUSH_STRIP_H

#include "brush.h"
#include "geo/gc_point.h"

class GCBrushStrip : public GCBrush
{
    uint16_t clr[2];
    uint8_t len1, len2;
    uint8_t period;
    GCPoint offset;
//    int ox, oy; // offsets x, y

public:
    GCBrushStrip(uint16_t c1_, uint16_t c2_, uint8_t len1_, uint8_t len2_, const GCPoint& offset_ = {0,0}) 
        : clr{c1_,c2_}, len1(len1_), len2(len2_), period(len1_+len2_), offset(offset_) 
    {
    }

    void setOffset(GCPoint const& offset_)
    {
        offset = offset_;
    }

    uint16_t color(int x, int y) const override
    {
        x -= offset.x;
        y -= offset.y;
        auto pos = (x+y) % period; 
        return pos < len1 ? clr[0] : clr[1];
    }

    void paint(GCRect const& rc, Device_ILI9341& device) const override
    {
        const int_fast16_t y1_ = (rc.y1() - offset.y);
        const int_fast16_t x1_ = (rc.x1() - offset.x);
        const uint_fast16_t y1 = y1_ < 0 ? (period - (-y1_%period)) : y1_ % period;
        const uint_fast16_t x1 = x1_ < 0 ? (period - (-x1_%period)) : x1_ % period;
        const uint_fast16_t yEnd = y1 + rc.height();
        const uint_fast16_t xEnd = x1 + rc.width();

        for (uint_fast16_t y = y1; y < yEnd; ++y) {
        for (uint_fast16_t x = x1; x < xEnd; ++x) {
            const auto pos = (x+y) % period;
            device.fastWrite(pos < len1 ? clr[0] : clr[1]);
            //device.fastWrite(clr[pos < len1]);
        }   
        }
    };

    void paint(GCPoint const& pt, Device_ILI9341& device) const override
    {
        const auto x = pt.x-offset.x;
        const auto y = pt.y-offset.y;
        const auto pos = (x+y) % period; 
        return device.fastWrite( pos < len1 ? clr[0] : clr[1] );
    };

    virtual GCBrush* clone() const { return new GCBrushStrip(*this); }
};

#endif // file__BRUSH_STRIP_H