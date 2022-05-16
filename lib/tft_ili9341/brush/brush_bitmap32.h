#pragma once
#ifndef file__BRUSH_BITMAP_32_H
#define file__BRUSH_BITMAP_32_H

#include "brush.h"

class GCBrushBitmap32 : public GCBrush
{
    const GCPoint pt;
    const uint8_t scale_x;
    const uint8_t scale_y;
    const GCBrush& brushFg;
    const GCBrush& brushBg;
    const uint32_t* bitmap;
    
    uint8_t w, h;


    bool isOutside(int x, int y) const {
        return x < 0 || y < 0 || x >= w || y >= h;
    }

public:
    explicit GCBrushBitmap32( const GCPoint& pt_, GCSize const& scale_
                            , const GCBrush& brushFg_, const GCBrush& brushBg_
                            , const uint32_t* bitmap_, uint8_t width, uint8_t height) 
    : pt(pt_), scale_x(scale_.sx), scale_y(scale_.sy)
    , brushFg(brushFg_), brushBg(brushBg_), bitmap(bitmap_), w(width), h(height) {}
    
    uint16_t color(int x, int y) const override
    {
        const int xx = x - pt.x;
        const int yy = y - pt.y;

        if (isOutside(xx, yy))
            return brushBg.color(x,y);

        const uint8_t bx = xx / scale_x;
        const uint8_t by = yy / scale_y;
        constexpr uint32_t one = 1 << 31;

        const bool isForeground = bitmap[by] & (one >> bx);
        return isForeground ? brushFg.color(x, y) : brushBg.color(x, y);
    }

    virtual GCBrush* clone() const { return new GCBrushBitmap32(*this); }

    void paint(GCRect const& rc, Device_ILI9341& device) const override
    {
        for (int y = rc.y1(); y <= rc.y2(); ++y)
        for (int x = rc.x1(); x <= rc.x2(); ++x)
        {
            const int xx = x - pt.x;
            const int yy = y - pt.y;

            if (isOutside(xx, yy)) {
                device.fastWrite(brushBg.color(x,y));
            }
            else {
                const uint8_t bx = xx / scale_x;
                const uint8_t by = yy / scale_y;
                constexpr uint32_t one = 1 << 31;

                const bool isForeground = bitmap[by] & (one >> bx);
                if (isForeground)
                    device.fastWrite(brushFg.color(x,y));
                else
                    device.fastWrite(brushBg.color(x,y));
            }
        }
    };
};

#endif // file__BRUSH_BITMAP_32_H
