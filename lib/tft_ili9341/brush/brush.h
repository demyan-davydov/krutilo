#pragma once
#ifndef file__BRUSH_H
#define file__BRUSH_H

#include <cstdint> // uint16_t
#include "device_ili9341.h"
#include "geo/gc_rect.h"


class Device_ILI9341;

class GCBrush
{
public:
    virtual ~GCBrush() = 0;
    virtual uint16_t color(int x, int y) const = 0;
    virtual GCBrush* clone() const = 0;

    virtual void paint(GCRect const& rc, Device_ILI9341& device) const {};
    virtual void paint(GCPoint const& pt, Device_ILI9341& device) const { paint(GCRect(pt), device); };
};

#endif // file__BRUSH_H
