/// Represent closed Rectange [x1..x2], [y1..y2]


#pragma once
#ifndef file__GEO_GCRECT_H
#define file__GEO_GCRECT_H

#include "gc_range.h"
#include "gc_point.h"
#include "gc_size.h"
#include "gc_coord.h"
#include "gc_extents2d.h"

class GCRect
{
public:
    /// construct empty rectange at point (x,y)
    static GCRect createEmpty(GCCoord x, GCCoord y) { return GCRect(GCRange::createEmpty(x), GCRange::createEmpty(y)); }

    /// construct empty rectange at point (pt)
    static GCRect createEmpty(GCPoint const& pt) { return GCRect(GCRange::createEmpty(pt.x), GCRange::createEmpty(pt.y)); }

    /// create rectangle with specified sizes
    static GCRect createSized(GCPoint const& pt, GCSize const& sz) { 
        return GCRect(pt.x, pt.y, pt.x+sz.sx-1, pt.y+sz.sy-1); 
    }

    /// create rectangle with specified sizes
    static GCRect createSized(GCCoord x, GCCoord y, GCCoord width, GCCoord height) { 
        return GCRect(x, y, x+width-1, y+height-1); 
    }

    /// create rectangle at specified center
    static GCRect createCentered(GCPoint const& pt, GCCoord rx, GCCoord ry) { 
        return GCRect(pt.x-rx, pt.y-ry, pt.x+rx, pt.y+ry); 
    }

    /// create rectangle at specified center
    static GCRect createCentered(GCCoord x, GCCoord y, GCCoord rx, GCCoord ry) { 
        return GCRect(x-rx, y-ry, x+rx, y+ry); 
    }


    /// construct rectange what include only point (x,y)
    GCRect(GCCoord x, GCCoord y) : rx(x), ry(y) {}
    
    /// construct rectange what include only point (pt)
    GCRect(const GCPoint& pt) : rx(pt.x), ry(pt.y) {}

    /// construct rect includes points X[x1..x2], Y[y1..y2]
    GCRect(GCCoord x1, GCCoord y1, GCCoord x2, GCCoord y2) : rx(x1,x2), ry(y1,y2) {}

    /// Construct rect 
    GCRect(const GCPoint& pt1, const GCPoint& pt2) : rx(pt1.x,pt2.x), ry(pt1.y,pt2.y) {}
    
    /// Construct rect
    GCRect(const GCRange& rx_, const GCRange& ry_) : rx(rx_), ry(ry_) {}

    /// is valid rect (both ranges are valid)
    bool valid() const { return rx.valid() && ry.valid(); }

    /// is rect empty (not includes any points). It also may be negative (a > b)
    bool empty() const { return rx.empty() || ry.empty(); }
    
    /// both ranges of rectangle is positive: non empty & valid
    bool positive() const { return rx.positive() && ry.positive(); }

    /// return valid rectangle (a <= b)
    GCRect validSwaped() const { return GCRect(rx.validSwaped(), ry.validSwaped()); }

    /// return valid rectangle (a <= b)
    GCRect validCollapsed() const { return GCRect(rx.validCollapsed(), ry.validCollapsed()); }

    /// validate ract is positive. If not swap ranges, to include same points but be positive
    void validateSwap() {rx.validateSwap(); ry.validateSwap(); }

    /// validate ract is positive. if not collapse range to empty range
    void validateCollapse() {rx.validateCollapse(); ry.validateCollapse(); }

    /// swap X and Y ranges
    void transpose() { std::swap(rx, ry); }

    /// return new rectangle with swapped X and Y ranges
    GCRect transposed() const { return GCRect(ry, rx); }

    /// absolute square, always positive
    GCCoord square() const { return rx.size()*ry.size(); }

    /// absolute square, always positive
    GCCoord squareAbs() const { return std::abs(rx.size()*ry.size()); }

    /// rect width, may be negative
    GCCoord width() const { return rx.size(); }

    /// rect height, may be negative
    GCCoord height() const { return ry.size(); }

    /// rect size, may be negative
    GCSize size() const { return GCSize{rx.size(), ry.size()}; }

    /// resize from topp left corner
    void resize(GCCoord sx, GCCoord sy) {rx.resizeFromA(sx); ry.resizeFromA(sy); }

    /// resize from topp left corner
    void resize(GCSize const& sz) { resize(sz.sx, sz.sy); }

    GCCoord x1() const {return rx.a;}
    GCCoord x2() const {return rx.b;}
    GCCoord y1() const {return ry.a;}
    GCCoord y2() const {return ry.b;}

    GCPoint topLeft() const { return GCPoint(rx.a, ry.a); }
    GCPoint topCenter() const { return GCPoint(rx.center(), ry.a); }
    GCPoint topRight() const { return GCPoint(rx.b, ry.a); }

    GCPoint midLeft() const { return GCPoint(rx.a, ry.center()); }
    GCPoint midCenter() const { return GCPoint(rx.center(), ry.center()); }
    GCPoint midRight() const { return GCPoint(rx.b, ry.center()); }

    GCPoint bottomLeft() const { return GCPoint(rx.a, ry.b); }
    GCPoint bottomCenter() const { return GCPoint(rx.center(), ry.b); }
    GCPoint bottomRight() const { return GCPoint(rx.b, ry.b); }
    
    GCPoint center() const { return midCenter(); }

    void moveTopLeft(const GCPoint& pt) {rx.moveATo(pt.x); ry.moveATo(pt.y); }
    void moveTopRigth(const GCPoint& pt) {rx.moveBTo(pt.x); ry.moveATo(pt.y); }
    void moveBottomLeft(const GCPoint& pt) {rx.moveATo(pt.x); ry.moveBTo(pt.y); }
    void moveBottomRigth(const GCPoint& pt) {rx.moveBTo(pt.x); ry.moveBTo(pt.y); }
    void moveCenter(const GCPoint& pt) {rx.moveCenterTo(pt.x); ry.moveCenterTo(pt.y); }

    /// is rectangle contain point?
    bool contain(GCCoord x, GCCoord y) const {return rx.contain(x) && ry.contain(y); }
    
    /// is rectangle contain point?
    bool contain(const GCPoint& pt) const {return rx.contain(pt.x) && ry.contain(pt.y); }
    
    /// is rectangle contain other rectangle?
    bool contain(const GCRect& rc) const {return rx.contain(rc.rx) && ry.contain(rc.ry); }

    /// expand rectangle to include specified point
    void eat(const GCPoint& pt) { rx.eat(pt.x); ry.eat(pt.y); }
    
    /// collapse to specified point
    void collapseTo( const GCPoint& pt ) { rx.collapseTo(pt.x); ry.collapseTo(pt.y); }
    
    /// offset rectangle by value
    void moveBy(const GCPoint& pt) { rx.moveBy(pt.x); ry.moveBy(pt.y); }

    /// return offseted rectangle by value
    GCRect movedBy(const GCPoint& pt) const { auto rc = *this; rc.moveBy(pt); return rc; }
    
    /// increase sizes 
    void increaseBy(GCCoord v) { rx.increaseBy(v); ry.increaseBy(v); }

    /// increase sizes 
    void increaseBy(GCSize const& v) { rx.increaseBy(v.sx); ry.increaseBy(v.sy); }

    /// increase sizes 
    void increaseBy(GCExtents2d const& e) { rx.increaseBy(e.x); ry.increaseBy(e.y); }


    /// decrease sizes 
    void decreaseBy(GCCoord v) { rx.decreaseBy(v); ry.decreaseBy(v); }

    /// decrease sizes 
    void decreaseBy(GCSize const& v) { rx.decreaseBy(v.sx); ry.decreaseBy(v.sy); }

    /// decrease sizes 
    void decreaseBy(GCExtents2d const& e) { rx.decreaseBy(e.x); ry.decreaseBy(e.y); }


    /// return increased rectangle 
    GCRect increasedBy(GCSize const& v) const { auto r = *this; r.increaseBy(v); return r; }

   /// return increased rectangle 
    GCRect increasedBy(GCExtents2d const& v) const { auto r = *this; r.increaseBy(v); return r; }

    /// return decreased rectangle 
    GCRect decreasedBy(GCSize const& v) const { auto r = *this; r.decreaseBy(v); return r; }

    /// return decreased rectangle 
    GCRect decreasedBy(GCExtents2d const& v) const { auto r = *this; r.decreaseBy(v); return r; }


    /// find intersection
    GCRect intersection( const GCRect& rc ) const { return GCRect(rx.intersection(rc.rx), ry.intersection(rc.ry)); }
    
    /// join rectangles
    GCRect commutation( const GCRect& rc ) const { return GCRect(rx.commutation(rc.rx), ry.commutation(rc.ry)); }

    bool operator == (GCRect const& o) const { return rx == o.rx && ry == o.ry; }
    bool operator != (GCRect const& o) const { return rx != o.rx || ry != o.ry; }
    GCRect& operator += (const GCPoint& pt) { moveBy(pt); return *this; }
    GCRect& operator -= (const GCPoint& pt) { moveBy(-pt); return *this; }
    GCRect operator + (const GCPoint& pt) const { auto rc = *this; rc.moveBy(pt); return rc; }
    GCRect operator - (const GCPoint& pt) const { auto rc = *this; rc.moveBy(-pt); return rc; }

    GCRect& operator = (GCRect const& o) = default;

    GCRange rx, ry;
};

#endif // file__GEO_GCRECT_H
