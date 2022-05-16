#pragma once
#ifndef file__BRUSH_SOLID_H
#define file__BRUSH_SOLID_H

#include "brush.h"

class GCBrushSolid : public GCBrush
{
    uint16_t m_color;

public:
    explicit GCBrushSolid(uint16_t c) : m_color(c) {}
    uint16_t color(int, int) const override
    {
        return m_color;
    }

    virtual GCBrush* clone() const { return new GCBrushSolid(*this); }

    void paint(GCRect const& rc, Device_ILI9341& device) const override
    {
        device.fastWriteN(m_color, rc.square());
    };

    void paint(GCPoint const& pt, Device_ILI9341& device) const override
    {
        device.fastWrite(m_color);
    };

    void color(uint16_t c)  { m_color = c; }

    uint16_t color() const { return m_color; }
};

#endif // file__BRUSH_SOLID_H
