
#pragma once
#ifndef file__GEO_GCSIZE_H
#define file__GEO_GCSIZE_H

#include "gc_coord.h"
#include <algorithm> // std::max()



class GCSize
{
public:
    GCSize(GCCoord sx_, GCCoord sy_) : sx(sx_), sy(sy_) {}

    void assign(GCCoord v) { sx = sy = v; }

    void increaseBy(GCSize const& o) { sx += o.sx; sy += o.sy; }

    void decreaseBy(GCSize const& o) { sx -= o.sx; sy -= o.sy; }

    GCCoord square() const { return sx*sy; }

    GCSize applyMinSize(GCSize const& sz) const { return GCSize(std::max(sx,sz.sx), std::max(sy,sz.sy)); }


    GCSize operator - () const { return GCSize{-sx, -sy}; }
    GCSize operator + (GCSize const& o) { auto t = *this; t.increaseBy(o); return t; };
    GCSize operator - (GCSize const& o) { auto t = *this; t.decreaseBy(o); return t; };

    GCSize& operator += (GCSize const& o) { increaseBy(o); return *this; };
    GCSize& operator -= (GCSize const& o) { decreaseBy(o); return *this; };

    bool operator == (GCSize const& o) const { return sx == o.sx && sy == o.sy; }
    bool operator != (GCSize const& o) const { return sx != o.sx || sy != o.sy; }
    GCSize& operator = (GCSize const& o) = default;

    GCCoord sx, sy;
};

#endif // file__GEO_GCSIZE_H
