#pragma once
#ifndef file__GEO_GCUTILS_H
#define file__GEO_GCUTILS_H

#include "gc_rect.h"
#include "gc_line.h"
#include "gc_coord.h"


/// find range for intersection beetwen rectangle (rect) and line,
/// write result to 'rx' and 'ry'
/// if any of ranges is empty, they are has no intersection
void intersectionRange(const GCRect& rect, const GCLine& line, GCRange& rx, GCRange& ry);

///  foregroudRange:       *-------*
///  backgroundRange:  *----------------*
///  result:           *--*         *---*
/// 
int crossRanges(GCRange const& backgroundRange, GCRange const& foregroudRange, GCRange bg[2]);

#endif // file__GEO_GCUTILS_H
