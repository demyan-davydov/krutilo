/// Represent closed range [a..b]

#pragma once
#ifndef file__GEO_GCRANGE_H
#define file__GEO_GCRANGE_H

#include <algorithm>
#include "gc_coord.h"
#include "gc_extents.h"


class GCRange
{
public:


    /// construct range what includes only point 'v' => [v]

    /// construct empty range at point 'v'
    static GCRange createEmpty(GCCoord v) { return GCRange(v, v-1); }

    /// Create range [a..b]
    //static GCRange createClosed(GCCoord a_, GCCoord b_) { return GCRange(v, v-1); }

    /// Create range [a..b]
    GCRange(GCCoord a_, GCCoord b_) : a(a_), b(b_) {}

    /// Create range what includes only point 'v' => [v]
    explicit GCRange(GCCoord v) : a(v), b(v) {}

    /// Copy c'tor
    GCRange(const GCRange& r) = default;


    /// assign value
    void assign(GCCoord a_, GCCoord b_) { a = a_; b = b_; }


    void collapseTo(GCCoord v) { a = v; b = v - 1; }
    void collapseToA() { collapseTo(a); }
    void collapseToB() { collapseTo(b); }
    void collapseToCenter() { collapseTo(center()); }
    //void assign(int v) { a = b = v; }

    void moveATo(GCCoord pt) { GCCoord sz = b-a; a = pt; b = a + sz; }
    void moveBTo(GCCoord pt) { GCCoord sz = b-a; b = pt; a = b - sz; }
    void moveCenterTo(GCCoord pt) { moveBy(pt - center()); }
    void moveBy(GCCoord offset) { a += offset; b += offset; }

    void resizeFromA(GCCoord newSize) { b = a + newSize - 1; }
    void resizeFromB(GCCoord newSize) { a = b - newSize + 1; }

    /// expand (move) both sides by specified value
    //void expandBy(GCCoord v) { a -= v; b +=v; }

    void increaseBy(GCExtents const& e) { a -= e.a; b += e.b; }

    void increaseBy(GCCoord v) { a -= v; b += v; }

    void decreaseBy(GCExtents const& e) { a += e.a; b -= e.b; }
    
    void decreaseBy(GCCoord v) { a += v; b -= v; }


    GCRange increasedBy(GCExtents const& e) const { auto t = *this; t.increaseBy(e); return t; }

    GCRange increasedBy(GCCoord v) const { auto t = *this; t.increaseBy(v); return t; }

    GCRange decreasedBy(GCExtents const& e) const { auto t = *this; t.decreasedBy(e); return t; }
    
    GCRange decreasedBy(GCCoord v) const { auto t = *this; t.decreasedBy(v); return t; }


    /// expand range to include specified point
    void eat(GCCoord pt) { if (pt < a) a = pt; else if (pt > b) b = pt; }



    /// return valid range (a <= b). If initial range is not return swaped range 
    GCRange validSwaped() const { return (b < a) ? GCRange(b,a) : GCRange(a,b); }

    /// return valid range (a <= b). If initial range is not return collapsed to center range
    GCRange validCollapsed() const { return (b < a) ? GCRange::createEmpty(center()) : GCRange(a,b); }

    /// 
    void validateSwap() { if (b < a) std::swap(a,b); }
    
    /// 
    void validateCollapse() { if (b < a) collapseToCenter(); }

    //// return: 
    /// -1 -- point leftest then range
    ///  0 -- point inside range
    ///  1 -- point rigtest then range
    int classify(GCCoord p) const { return p < a ? -1 : (p > b ? 1 : 0); }
    
    /// is specified point inside range?
    //bool inside(GCCoord p) const { return p >= a && p <= b; }

    /// is specified ranges intersects?
    bool intersects(const GCRange& r) const { return a < r.a ? (b >= r.a) : (r.b >= a); }

    /// is range contain other range?
    bool contain(const GCRange& r) const {return a <= r.a && b >= r.b; }
    
    /// is range contain point?
    bool contain(GCCoord p) const { return a <= p && b >= p; }

    /// is range contain point? current range may be invalid
    //bool containInvalid(GCCoord v) const { return (a <= v) == (b >= v); }

    /// return renage intersection, return empty range if ranges are not intersected
    /// in case of empty intersection returned range may be invalid
    /// Both ranges must be valid
    GCRange intersection(const GCRange& r) const 
    {
        return GCRange(std::max(a, r.a), std::min(b, r.b));

        /*if (a < r.a) {
            return b < r.a ? GCRange::createEmpty(a) : GCRange(r.a, std::min(b, r.b));
        }
        else {
            return r.b < a ? GCRange::createEmpty(r.a) : GCRange(a, std::min(b, r.b));
        }*/
    }

    /// Build range containing both ranges
    /// suppose both ranges are valid
    GCRange commutation(const GCRange& r) const 
    {
        return GCRange(std::min(a,r.a), std::max(b, r.b));
    }

    /// is valid range (empty or positive)
    bool valid() const { return size() >= 0; }

    /// is range is empty (size == 0). 
    /// invalid ranges also supposed as empty()
    //bool empty() const { return size() == 0; }
    bool empty() const { return b < a; }

    /// normal not empty range, contain at least one element (size() > 0)
    bool positive() const { return a <= b; }

    /// size of range
    GCCoord size() const { return b - a + 1; }

    /// 
    GCCoord sizeAbs() const { return std::abs(size()); }

    /// center of range
    GCCoord center() const { return (a+b)/2; }

    //friend bool operator == (GCRange const&, GCRange const&) = default;
    bool operator == (GCRange const& o) const { return a == o.a && b == o.b; }
    bool operator != (GCRange const& o) const { return a != o.a || b != o.b; }
    GCRange& operator = (GCRange const& o) = default;

    GCRange operator + (GCExtents const& e) const { return GCRange{a-e.a, b+e.b }; }
    GCRange operator - (GCExtents const& e) const { return GCRange{a+e.a, b-e.b }; }

    GCCoord a, b;
};


#endif // file__GEO_GCRANGE_H
