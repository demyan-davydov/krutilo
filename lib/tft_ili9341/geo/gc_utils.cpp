#include "gc_utils.h"
#include <algorithm>

void intersectionRange(const GCRect& rect, const GCLine& line, GCRange& rx, GCRange& ry)
{
    // todo: implementation
}


///  foregroudRange:       *-------*       |   *----------*
///  backgroundRange:  *----------------*  |      *------------*
///  result:           *--*         *---*  |               *---*
/// 
int crossRanges(GCRange const& bgRange, GCRange const& fgRange, GCRange bg[2])
{
    int i = 0;
    if (bgRange.a < fgRange.a) {
        bg[i++].assign(bgRange.a, std::min(bgRange.b, (GCCoord)(fgRange.a-1)));
    }

    if (bgRange.b > fgRange.b) {
        bg[i++].assign(std::max(bgRange.a, (GCCoord)(fgRange.b+1)), bgRange.b);
    }

    return i;
}

/*
int crossRanges(GCRect const& bg, GCRect const& fg, GCRect bgs[4])
{

    GCRange crossX[2] = {{0,0}, {0,0}};
    GCRange crossY[2] = {{0,0}, {0,0}};
    int crossesX = crossRanges(bg.rx, fg.rx, crossX);
    int crossesY = crossRanges(bg.ry, fg.ry, crossY);

    return 0;
}
*/