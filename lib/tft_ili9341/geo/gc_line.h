/// Represent closed Line [x1..x2] -> [y1..y2]


#pragma once
#ifndef file__GEO_GCLINE_H
#define file__GEO_GCLINE_H

#include "gc_coord.h"
#include "gc_point.h"
#include "gc_range.h"
#include <algorithm>

class GCLine
{
public:
    GCLine(const GCPoint& a_, const GCPoint& b_) : a(a_), b(b_) {}
    GCLine(GCCoord x1, GCCoord y1, GCCoord x2, GCCoord y2) : a(x1,y1), b(x2,y2) {}

    GCCoord squaredLength() const { return (b-a).squaredDistance(); }
    GCPoint center() const { return GCPoint( (a.x+b.x)/2, (a.y+b.y)/2); }
    GCRange rangeX() const { return GCRange(a.x, b.x); }
    GCRange rangeY() const { return GCRange(a.y, b.y); }
    GCPoint vector() const { return b-a; }

    void swapEnds() { std::swap(a,b); }

    bool operator == (GCLine const& o) const { return a == o.a && b == o.b; }
    bool operator != (GCLine const& o) const { return a != o.a || b != o.b; }
    GCLine& operator = (GCLine const& o) = default;

public:
    GCPoint a, b;
};

#endif // file__GEO_GCLINE_H
