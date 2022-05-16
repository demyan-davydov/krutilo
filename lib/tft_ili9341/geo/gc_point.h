/// Represent point with integer coordinates

#pragma once
#ifndef file__GEO_GCPOINT_H
#define file__GEO_GCPOINT_H

#include "gc_coord.h"



class GCPoint_X
{
public:
    GCCoord x;

    GCPoint_X(GCCoord x_) noexcept : x(x_) {}
    explicit operator GCCoord&() noexcept { return x; }
    explicit operator const GCCoord&() const noexcept { return x; }
    GCPoint_X operator - () const { return GCPoint_X{-x}; }
};

class GCPoint_Y
{
public:
    GCCoord y;

    GCPoint_Y(GCCoord y_) noexcept : y(y_) {}
    explicit operator GCCoord&() noexcept { return y; }
    explicit operator const GCCoord&() const noexcept { return y; }
    GCPoint_Y operator - () const { return GCPoint_Y{-y}; }
};

class GCPoint
{
public:
    GCPoint(GCCoord x_, GCCoord y_) noexcept : x(x_), y(y_) {}
    GCPoint(GCPoint_X v) noexcept : x(v.x), y(0) {}
    GCPoint(GCPoint_Y v) noexcept : x(0), y(v.y) {}

    void moveBy(const GCPoint_X& pt) { x += pt.x; }
    void moveBy(const GCPoint_Y& pt) { y += pt.y; }
    void moveBy(GCCoord dx, GCCoord dy) { x += dx; y += dy; }
    void moveBy(const GCPoint& pt) { moveBy(pt.x, pt.y); }

    GCPoint movedBy(GCCoord dx, GCCoord dy) const { auto t = *this; t.moveBy(dx, dy); return t; }
    GCPoint movedBy(const GCPoint& pt) const { auto t = *this; t.moveBy(pt); return t; }

    GCCoord squaredDistance() const { return x*x + y*y; }
    GCCoord squaredDistanceTo(GCPoint const& pt) const { return ((*this)-pt).squaredDistance(); }
    GCCoord squaredDistanceTo(int x, int y) const { return ((*this)-GCPoint(x,y)).squaredDistance(); }
 
    void scale(GCCoord v) { x *= v, y *= v; }
    void descale(GCCoord v) { x /= v, y /= v; }

    // unary -
    GCPoint operator - () const { return GCPoint{-x, -y}; }

    GCPoint operator + (const GCPoint& pt) const { return GCPoint(x+pt.x, y+pt.y); }
    GCPoint operator - (const GCPoint& pt) const { return GCPoint(x-pt.x, y-pt.y); }
    GCPoint& operator += (const GCPoint& pt) { x+=pt.x; y+=pt.y; return *this; }
    GCPoint& operator -= (const GCPoint& pt) { x-=pt.x; y-=pt.y; return *this; }
    //GCPoint& operator *= (const GCPoint& pt) { x*=pt.x; y*=pt.y; return *this; }
    //GCPoint& operator /= (const GCPoint& pt) { x/=pt.x; y/=pt.y; return *this; }

    GCPoint operator + (const GCPoint_X& pt) const { return GCPoint(x+pt.x, y); }
    GCPoint operator - (const GCPoint_X& pt) const { return GCPoint(x-pt.x, y); }
    GCPoint operator + (const GCPoint_Y& pt) const { return GCPoint(x, y+pt.y); }
    GCPoint operator - (const GCPoint_Y& pt) const { return GCPoint(x, y-pt.y); }

    bool operator == (GCPoint const& o) const { return x == o.x && y == o.y; }
    bool operator != (GCPoint const& o) const { return x != o.x || y != o.y; }
    GCPoint& operator = (GCPoint const& o) = default;

    GCCoord x, y;
};

#endif // file__GEO_GCPOINT_H
