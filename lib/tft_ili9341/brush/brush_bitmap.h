#pragma once
#ifndef file__BRUSH_BITMAP_H
#define file__BRUSH_BITMAP_H

#include "brush.h"
#include "bitmap_view.h"


class GCBrushBitmap : public GCBrush
{
    const GCPoint pt;
    const uint8_t scale_x;
    const uint8_t scale_y;
    const GCBrush& brushFg;
    const GCBrush& brushBg;
    const ConstBitmapView bmp;

    bool isOutside(int x, int y) const {
        return x < 0 || y < 0 || x >= bmp.width() || y >= bmp.height();
    }

public:
    explicit GCBrushBitmap( const GCPoint& pt_, GCSize const& scale_
                          , const GCBrush& brushFg_, const GCBrush& brushBg_
                          , const ConstBitmapView& bmp_) 
    : pt(pt_), scale_x(scale_.sx), scale_y(scale_.sy)
    , brushFg(brushFg_), brushBg(brushBg_), bmp(bmp_)
    {

    }
    
    uint16_t color(int x, int y) const override
    {
        const int xx = x - pt.x;
        const int yy = y - pt.y;

        if (isOutside(xx, yy))
            return brushBg.color(x,y);

        const uint16_t bx = xx / scale_x;
        const uint16_t by = yy / scale_y;
        const bool isForeground = bmp.pixel(bx, by);
        return isForeground ? brushFg.color(x, y) : brushBg.color(x, y);
    }

    virtual GCBrush* clone() const { return new GCBrushBitmap(*this); }

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
                const uint16_t bx = xx / scale_x;
                const uint16_t by = yy / scale_y;
                const auto c = bmp.pixel(bx, by) ? brushFg.color(x,y) : brushBg.color(x,y);
                device.fastWrite(c);
            }
        }
    };
};

#endif // file__BRUSH_BITMAP_H
