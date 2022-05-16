#pragma once
#ifndef file__GEO_GC_EXTENTS2D_H
#define file__GEO_GC_EXTENTS2D_H

#include "gc_extents.h"
#include "gc_size.h"
#include "gc_point.h"
#include <algorithm>

/// Describe extents for GCRect

class GCExtents2d
{
public:
    GCExtents x; ///< extents for 'x' dimension
    GCExtents y; ///< extents for 'y' dimension

    GCSize toSize() const { return GCSize{x.sum(), y.sum()}; }

    GCPoint topLeft() const { return GCPoint{x.a, y.a}; }

    void assign(GCCoord x_, GCCoord y_) { x.assign(x_); y.assign(y_); }

    void assign(GCCoord v) { x.assign(v); y.assign(v); }

    void increaseBy(GCCoord v) { x.increaseBy(v); y.increaseBy(v); }

    void decreaseBy(GCCoord v) { x.decreaseBy(v); y.decreaseBy(v); }



    /// Swap 'x' and 'y' extents
    void transpose() { std::swap(x, y); }

    // unary -
    GCExtents2d operator - () const { return {-x, -y}; }

/*
    GCExtents operator + (GCExtents const& o) const { return GCExtents{a+o.a, b+o.b }; }
    GCExtents operator - (GCExtents const& o) const { return GCExtents{a-o.a, b-o.b }; }
    GCExtents operator + (GCCoord v) const { return GCExtents{a+v, b+v }; }
    GCExtents operator - (GCCoord v) const { return GCExtents{a-v, b-v }; }

    GCExtents& operator += (GCExtents const& e) { increaseBy(e); return *this; }
    GCExtents& operator -= (GCExtents const& e) { decreaseBy(e); return *this; }
    GCExtents& operator += (GCCoord v) { increaseBy(v); return *this; }
    GCExtents& operator -= (GCCoord v) { decreaseBy(v); return *this; }
*/
    bool operator == (GCExtents2d const& o) const { return x == o.x && y == o.y; }
    bool operator != (GCExtents2d const& o) const { return x != o.x || y != o.y; }
    GCExtents2d& operator = (GCExtents2d const& o) = default;
};

#endif // file__GEO_GC_EXTENTS2D_H