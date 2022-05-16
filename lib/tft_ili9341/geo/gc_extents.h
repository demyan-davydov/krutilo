#pragma once
#ifndef file__GEO_GC_EXTENTS_H
#define file__GEO_GC_EXTENTS_H

#include "gc_coord.h"

/// Describe extents for GCRange

class GCExtents
{
public:
    GCCoord a; ///< 'left' extent
    GCCoord b; ///< 'right' extent

    GCCoord sum() const { return a + b; }

    void assign(GCCoord a_, GCCoord b_) { a = a_, b = b_; }

    void assign(GCCoord v) { a = b = v; }

    void increaseBy(GCExtents const& e) { a += e.a; b += e.b; }

    void increaseBy(GCCoord v) { a += v; b += v; }

    void decreaseBy(GCExtents const& e) { a -= e.a; b -= e.b; }
    
    void decreaseBy(GCCoord v) { a -= v; b -= v; }

    // unary -
    GCExtents operator - () const { return {-a, -b}; }

    GCExtents operator + (GCExtents const& o) const { return GCExtents{a+o.a, b+o.b }; }
    GCExtents operator - (GCExtents const& o) const { return GCExtents{a-o.a, b-o.b }; }
    GCExtents operator + (GCCoord v) const { return GCExtents{a+v, b+v }; }
    GCExtents operator - (GCCoord v) const { return GCExtents{a-v, b-v }; }

    GCExtents& operator += (GCExtents const& e) { increaseBy(e); return *this; }
    GCExtents& operator -= (GCExtents const& e) { decreaseBy(e); return *this; }
    GCExtents& operator += (GCCoord v) { increaseBy(v); return *this; }
    GCExtents& operator -= (GCCoord v) { decreaseBy(v); return *this; }

    bool operator == (GCExtents const& o) const { return a == o.a && b == o.b; }
    bool operator != (GCExtents const& o) const { return a != o.a || b != o.b; }
    GCExtents& operator = (GCExtents const& o) = default;
};

#endif // file__GEO_GC_EXTENTS_H