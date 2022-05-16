#include "surface_ili9341.h"

#include "brush/brush_solid.h"
#include "brush/brush_bitmap.h"
#include "brush/brush_bitmap32.h"
#include "bitmap_view.h"

//#include <functional> // std::function, std::bind

#include "utils/bit_stream.h"

//#include <string.h>
#include <algorithm>

#include <stdarg.h> // vsprintf


//#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
typedef Surface_ILI9341 self_t;


Surface_ILI9341::Surface_ILI9341(Device_ILI9341& d)
: m_device(d)
{
    state().clip = screenRect();

    // standard brushes

    state().brushFg.reset(new GCBrushSolid(Clr_BLACK)); // foreground brush
    state().brushBg.reset(new GCBrushSolid(Clr_WHITE)); // background brush
    state().brushTextFg.reset(new GCBrushSolid(Clr_BLACK)); // foreground brush
    //brushTextBg; // background brush
}

void Surface_ILI9341::invertDisplay(bool i)
{
    //device.setAddrWindow({30,30,80,80});
    m_device.invertDisplay(i);
}

static bool isVerticalOrientation(Device_ILI9341::Orientation o)
{
    return  o == Device_ILI9341::Orient_Normal || 
            o == Device_ILI9341::Orient_180CW;
}

void Surface_ILI9341::orientation(Orientation orient)
{
    if (orient != state().orientation) {
        // transform current clip rect according to new orientation
        if (isVerticalOrientation(orient) != isVerticalOrientation(state().orientation)) {
            state().clip.transpose();
        }

        m_device.orientation(orient);
        state().orientation = orient;// device.orientation();
    }
}

Surface_ILI9341::Orientation Surface_ILI9341::orientation() const
{
    return state().orientation;
}

GCSize Surface_ILI9341::screenSize() const
{
    return m_device.screenSize(state().orientation);
}

GCRect Surface_ILI9341::screenRect() const
{
    return m_device.screenRect(state().orientation)-state().translate;
}

/*
void Surface_ILI9341::scrollTo(uint16_t y)
{
    state().scrollY = y;
    m_device.scrollTo(y);
}

uint16_t Surface_ILI9341::scroll() const
{
    return state().scrollY;
}
*/

void Surface_ILI9341::resetClip()
{
    state().clip = screenRect();
}

void Surface_ILI9341::clip(const GCRect& rc)
{
    state().clip = rc.intersection( screenRect() );
}

GCRect const& Surface_ILI9341::clip() const
{
    return state().clip;
}

void Surface_ILI9341::translate(GCPoint const& pt)
{
    translateBy(pt - state().translate);
    //state().translate = pt;
}

void Surface_ILI9341::translateBy(GCPoint const& pt)
{
    state().clip.moveBy(-pt);
    state().translate.moveBy(pt);
}

GCPoint Surface_ILI9341::translate() const
{
    return state().translate;
}

void Surface_ILI9341::translateBrush(GCPoint const& pt)
{
    state().translateBrush = pt;
}

void Surface_ILI9341::translateBrushBy(GCPoint const& pt)
{
    state().translateBrush.moveBy(pt);
}

GCPoint Surface_ILI9341::translateBrush() const
{
    return state().translateBrush;
}


void Surface_ILI9341::fillScreen(uint16_t color)
{
    h_renderRect(clip(), GCBrushSolid(color));
    m_device.clearRx();
}

void Surface_ILI9341::drawRect(GCRect const& rect, GCBrush const& brush)
{
    const auto rc = rect.validSwaped();
    const auto h = rc.height();
    const auto w = rc.width();
    const int16_t hSides = h - 2;
    const bool needDrawSides = hSides > 0;
    const bool needDrawBottom = h > 1;

    // top
    {
        h_drawHLine(rc.x1(), rc.y1(), w, brush);
    }
    if (needDrawBottom) {
        h_drawHLine(rc.x1(), rc.y2(), w, brush);
    }
    if (needDrawSides) {
        h_drawVLine(rc.x1(), rc.y1()+1, hSides, brush);
        h_drawVLine(rc.x2(), rc.y1()+1, hSides, brush);
    }
    m_device.clearRx();
}

void Surface_ILI9341::drawRect(GCRect const& rect)
{
    if (auto brush = state().brushFg.get()) {
        drawRect(rect, *brush);
    }
}

void Surface_ILI9341::fillRect(GCRect const& rc, GCBrush const& brush)
{
    h_renderRect(rc, brush);
    m_device.clearRx();
}

void Surface_ILI9341::fillRect(GCRect const& rc)
{
    if (auto brush = state().brushBg.get()) {
        h_renderRect(rc, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::fillExtentsInner(GCRect const& rc, GCExtents2d const& ext, GCBrush const& brush)
{
    fillExtents(rc.decreasedBy(ext), ext, brush);
}

void Surface_ILI9341::fillExtents(GCRect const& rc, GCExtents2d const& ext, GCBrush const& brush)
{
    const GCRect rc2 = rc.increasedBy(ext);
    const GCPoint p1 = rc2.topLeft(); // top
    const GCPoint p2 = rc2.bottomLeft() - GCPoint_Y(ext.y.b-1); // bottom
    const GCPoint p3 = rc2.topLeft() + GCPoint_Y(ext.y.a); // left
    const GCPoint p4 = rc.topRight() + GCPoint_X(1); // rigth

    fillRect(GCRect::createSized(p1, {rc2.width(), ext.y.a}), brush); // fill top
    fillRect(GCRect::createSized(p2, {rc2.width(), ext.y.b}), brush); // fill bottom
    fillRect(GCRect::createSized(p3, {ext.x.a, rc.height()}), brush); // fill left
    fillRect(GCRect::createSized(p4, {ext.x.b, rc.height()}), brush); // fill right
}

void Surface_ILI9341::fillExtentsRound(GCRect const& rc, int r, GCExtents2d const& ext, GCBrush const& brush, GCBrush const* brushBg)
{
    if (r < 1 || ext.x.a < r || ext.x.b < r ) {
        fillExtents(rc, ext, brush);
        return;
    }

    const GCRect rc2 = rc.increasedBy(ext);

    GCRect rcL = rc2; rcL.rx.resizeFromA(r);
    GCRect rcR = rc2; rcR.rx.resizeFromB(r);

    h_fillRoundedHelper(rcL, true, brush, brushBg);
    h_fillRoundedHelper(rcR, false, brush, brushBg);

    fillExtents(rc, GCExtents2d{ {ext.x.a-r, ext.x.b-r}, ext.y }, brush);
}

void Surface_ILI9341::drawLine(GCLine const& line, GCBrush const& brush)
{
    h_drawLine(line, brush);
    m_device.clearRx();
}

void Surface_ILI9341::drawLine(GCLine const& line)
{
    if (auto brush = state().brushFg.get()) {
        drawLine(line, *brush);
    }
}

void Surface_ILI9341::drawLine(GCPoint const& a, GCPoint const& b)
{
    drawLine({a,b});
}

void Surface_ILI9341::drawLine(GCCoord x0, GCCoord y0, GCCoord x1, GCCoord y1)
{
    drawLine(GCLine{x0, y0, x1, y1});
}

void Surface_ILI9341::drawVLine(GCPoint const& pt, GCCoord h, GCBrush const& brush)
{
    h_drawVLine(pt, h, brush);
    m_device.clearRx();
}

void Surface_ILI9341::drawVLine(GCPoint const& pt, GCCoord h)
{
    if (auto brush = state().brushFg.get()) {
        drawVLine(pt, h, *brush);
    }
}

void Surface_ILI9341::drawVLine(GCCoord x, GCCoord y, GCCoord h)
{
    if (auto brush = state().brushFg.get()) {
        drawVLine(GCPoint{x, y}, h, *brush);
    }
}

void Surface_ILI9341::drawHLine(GCPoint const& pt, GCCoord w, GCBrush const& brush)
{
    h_drawHLine(pt, w, brush);
    m_device.clearRx();
}

void Surface_ILI9341::drawHLine(GCPoint const& pt, GCCoord w)
{
    if (auto brush = state().brushFg.get()) {
        drawHLine(pt, w, *brush);
    }
}

void Surface_ILI9341::drawHLine(GCCoord x, GCCoord y, GCCoord w)
{
    if (auto brush = state().brushFg.get()) {
        drawHLine(GCPoint(x, y), w, *brush);
    }
}

void Surface_ILI9341::drawCircle(GCPoint const& pt, GCCoord r, GCBrush const& brush)
{
    h_drawCircle(pt, r, brush);
    m_device.clearRx();
}

void Surface_ILI9341::drawCircle(GCPoint const& pt, GCCoord r)
{
    if (auto brush = state().brushFg.get()) {
        h_drawCircle(pt, r, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::drawCircle(GCCoord x0, GCCoord y0, GCCoord r)
{
    if (auto brush = state().brushFg.get()) {
        h_drawCircle({x0, y0}, r, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::fillCircle(GCPoint const& pt, GCCoord r, GCBrush const& brush) 
{
    h_fillCircle(pt, r, brush);
    m_device.clearRx();
}

void Surface_ILI9341::fillCircle(GCPoint const& pt, GCCoord r) 
{
    if (auto brush = state().brushBg.get()) {
        h_fillCircle(pt, r, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::fillCircle(GCCoord x0, GCCoord y0, GCCoord r) 
{
    if (auto brush = state().brushBg.get()) {
        h_fillCircle({x0, y0}, r, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::drawTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3, GCBrush const& brush)
{
    h_drawLine({p1, p2}, brush);
    h_drawLine({p2, p3}, brush);
    h_drawLine({p3, p1}, brush);
    m_device.clearRx();
}

void Surface_ILI9341::drawTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3)
{
    if (auto brush = state().brushFg.get()) {
        h_drawLine({p1, p2}, *brush);
        h_drawLine({p2, p3}, *brush);
        h_drawLine({p3, p1}, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::drawTriangle(GCCoord x0, GCCoord y0, GCCoord x1, GCCoord y1, GCCoord x2, GCCoord y2)
{
    if (auto brush = state().brushFg.get()) {
        h_drawLine(GCLine(x0, y0, x1, y1), *brush);
        h_drawLine(GCLine(x1, y1, x2, y2), *brush);
        h_drawLine(GCLine(x2, y2, x0, y0), *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::fillTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3, GCBrush const& brush)
{
    h_fillTriangle(p1, p2, p3, brush);
    m_device.clearRx();
}

void Surface_ILI9341::fillTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3)
{
    if (auto brush = state().brushBg.get()) {
        h_fillTriangle(p1, p2, p3, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::fillTriangle(GCCoord x0, GCCoord y0, GCCoord x1, GCCoord y1, GCCoord x2, GCCoord y2)
{
    if (auto brush = state().brushBg.get()) {
        h_fillTriangle({x0, y0}, {x1, y1}, {x2, y2}, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::drawRoundRect(GCRect const& rc, GCCoord r, GCBrush const& brush)
{
    h_drawRoundRect(rc, r, brush);
    m_device.clearRx();
}

void Surface_ILI9341::drawRoundRect(GCRect const& rc, GCCoord r)
{
    if (auto brush = state().brushFg.get()) {
        h_drawRoundRect(rc, r, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::fillRoundRect(GCRect const& rc, GCCoord r, GCBrush const& brush)
{
    h_fillRoundRect(rc, r, brush);
    m_device.clearRx();
}

void Surface_ILI9341::fillRoundRect(GCRect const& rc, GCCoord r)
{
    if (auto brush = state().brushBg.get()) {
        h_fillRoundRect(rc, r, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::drawBitmap(GCPoint const& pt, const uint8_t *bitmap, uint16_t w, uint16_t h)
{
    if (auto brush = state().brushFg.get()) {
        h_drawBitmap(pt, bitmap, w, h, *brush);
        m_device.clearRx();
    }
}

void Surface_ILI9341::drawXBitmap(GCPoint const& pt, const uint8_t *bitmap,  uint16_t w, uint16_t h)
{
    if (auto brush = state().brushTextFg.get()) {
        h_drawXBitmap(pt, bitmap, w, h, *brush);
        m_device.clearRx();
    }
}

/*void Surface_ILI9341::drawChar(char c, GCPoint const& pt)
{
    auto* font = state().font;
    auto* brush = state().brushTextFg.get();
    GCBrush const* brushBg = state().brushTextBg.get();

    if (font && brush) {
        TextRenderCtx ctx {*font, state().textScale, *brush, brushBg, state().textStyle};
        h_drawChar(ctx, pt, c);
        ///h_drawChar(*font, pt, c, state().textScale, state().textStyle, *brush, brushBg);
    }
}*/

uint8_t Surface_ILI9341::drawChar(char c, GCPoint const& pt, GFXfont const& font, GCBrush const& brushFg, GCBrush const* brushBg)
{
    TextRenderCtx ctx {font, state().textScale, brushFg, brushBg, state().textStyle};
    return h_drawChar(ctx, pt, c);
}

static uint8_t glyphWidth(GFXfont const& font, uint8_t glyph, uint8_t tabSpaces) 
{
    return font.glyphRef(glyph).xAdvance;
    /*auto const& g = font.glyph;
    if (glyph < font.first || glyph > font.last) {
        return g[0].xAdvance;
    }
    else {
        if (glyph == (uint8_t)'\t') { // return repeat count?
            return g[(uint8_t)' ' - font.first].xAdvance * tabSpaces;
        }
        return g[glyph - font.first].xAdvance;
    }*/
}

/// is whitespace symbol
static bool isWs(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static const char* skipWs(const char* str, const char* strEnd)
{
    while (str != strEnd && isWs(*str)) {
        ++str;
    }
    return str;
}


/*uint8_t self_t::measureCharWidth(GFXfont const& font, char c) const
{
    return font.glyphRef(c).xAdvance;
}*/

GCSize self_t::measureChar(GFXfont const& font, char c) const
{
    return GCSize(font.glyphRef(c).xAdvance, font.yAdvance);
}

// todo: need we handle '\r' and drop last length?
static GCCoord measureText_(GFXfont const& font, const char* str, const char* strEnd) 
{
    GCCoord res = 0;
    while (str != strEnd) {
        res += font.glyphRef(*str++).xAdvance;
    }
    return res;
}

static GCCoord measureText_(GFXfont const& font, const char* str) 
{
    GCCoord res = 0;
    while (*str) {
        res += font.glyphRef(*str++).xAdvance;
    }
    return res;
}

GCSize self_t::measureTextLine(GFXfont const& font, const char* str, uint16_t len) const
{
    GCSize sz(0,0);

    if (str && len) {
        sz.sx = measureText_(font, str, str+len);
        sz.sy = font.yAdvance;
    }

    return sz;
}

GCSize self_t::measureTextLine(GFXfont const& font, const char* str) const
{
    GCSize sz(0,0);
    if (str) {
        sz.sx = measureText_(font, str);
        sz.sy = font.yAdvance;
    }
    return sz;
}

GCPoint self_t::drawTextMulti(const char* str, uint16_t strLen, GCRect const& rc, GCPoint const& start, int tabSpaces) 
{
    // todo: implement
    return GCPoint(0,0);
}

GCPoint self_t::drawTextSingle(const char* str, uint16_t len, GCPoint const& start, GFXfont const& font, GCBrush const& brushFg, GCBrush const* brushBg) 
{
    GCPoint pt = start;// + state().translate;

    if (str && len) {
        TextRenderCtx ctx {font, state().textScale, brushFg, brushBg, state().textStyle};
        const char* strEnd = str + len;
        while (str != strEnd) {
            pt.x += h_drawChar(ctx, pt, *str++);
        }
    }

    return pt;
}

GCPoint self_t::drawTextSingle(const char* str, GCPoint const& start, GFXfont const& font, GCBrush const& brushFg, GCBrush const* brushBg) 
{
    return drawTextSingle(str, strlen(str), start, font, brushFg, brushBg);
}

GCPoint self_t::drawTextSingle(const char* str, uint16_t len, GCPoint const& start) 
{
    GCPoint pt = start;// + state().translate;

    if (state().font && state().brushTextFg) {
        pt = drawTextSingle(str, len, start, *state().font, *state().brushTextFg.get(), state().brushTextBg.get());
    }

    return pt;
}

GCPoint self_t::drawTextSingle(const char* str, uint16_t len, GCRect rc, Anchor innerAnchor, GCPoint textOffset,
                               GFXfont const& font, GCBrush const& brushFg, GCBrush const* brushBg)
{
    const GCSize text_sz = measureTextLine(font, str, len);
    GCPoint const pt = placeInside(rc, text_sz, innerAnchor) + textOffset;

    auto prev_clip = clip();
    clip(clip().intersection(rc));

    // draw text
    drawTextSingle(str, len, pt, font, brushFg, brushBg);

    if (brushBg)
    {
        // fill top strip
        {
            GCRect r2 = rc; r2.ry.b = pt.y - 1;
            fillRect(r2, *brushBg);
        }
    
        // fill bottom strip
        {
            GCRect r2 = rc; r2.ry.a = pt.y + text_sz.sy;
            fillRect(r2, *brushBg);
        }

        // fill left & right 
        {
            GCRect r2 = rc; 
            r2.ry.a = pt.y;
            r2.ry.b = pt.y + text_sz.sy - 1;

            r2.rx.b = pt.x - 1;
            fillRect(r2, *brushBg);

            r2.rx.a = pt.x + text_sz.sx;
            r2.rx.b = rc.rx.b;
            fillRect(r2, *brushBg);
        }
    }

    // restore clip
    clip(prev_clip);
}


self_t& self_t::selectPen( GCBrush const& brush ) {
    state().brushFg.reset(brush.clone());
    return *this;
}

self_t& self_t::selectBrush( GCBrush const& brush ) {
    state().brushBg.reset(brush.clone());
    return *this;
}

self_t& self_t::selectTextColor( GCBrush const& brush ) {
    state().brushTextFg.reset(brush.clone());
    return *this;
}

self_t& self_t::selectTextBgColor( GCBrush const& brush) {
    state().brushTextBg.reset(brush.clone());
    return *this;
}



self_t& self_t::selectFont( GFXfont const& font ) {
    state().font = &font;
    return *this;
}

self_t& self_t::selectTextStyle(bool italic, bool underline) {
    state().textStyle = (italic ? TS_Italic : 0) | (underline ? TS_Underline : 0);
    return *this;
}

self_t& self_t::selectTextSize(uint8_t sz) {
    state().textScale.assign(sz);
    return *this;
}





// ----------------------------------------------------------------------------
// --- Helper methods  NEW
// ----------------------------------------------------------------------------

/// todo: clipping
///
void self_t::h_drawLine(GCLine const& line, GCBrush const& brush)
{
    int16_t x0 = line.a.x;
    int16_t y0 = line.a.y;
    int16_t x1 = line.b.x;
    int16_t y1 = line.b.y;

    int16_t steep =  std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; ++x0) {
        if (steep) {
            h_renderPixel(y0, x0, brush);
        } else {
            h_renderPixel(x0, y0, brush);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void self_t::h_fillRect(GCRect const& rc, GCBrush const& brush)
{
    h_renderRect(rc, brush);
}

void self_t::h_drawHLine(GCPoint const& pt, GCCoord w, GCBrush const& brush)
{
    h_renderRect(GCRect(pt.x, pt.y, pt.x+w-1, pt.y), brush);
}

void self_t::h_drawHLine(GCCoord x, GCCoord y, GCCoord w, GCBrush const& brush)
{
    h_renderRect(GCRect(x, y, x+w-1, y), brush);
}

void self_t::h_drawVLine(GCPoint const& pt, GCCoord h, GCBrush const& brush)
{
    h_renderRect(GCRect(pt.x, pt.y, pt.x, pt.y+h-1), brush);
}

void self_t::h_drawVLine(GCCoord x, GCCoord y, GCCoord h, GCBrush const& brush)
{
    h_renderRect(GCRect(x,y,x,y+h-1), brush);
}

void self_t::h_drawCircle(GCPoint const& pt, GCCoord r, GCBrush const& brush)
{
    const GCCoord x0 = pt.x;
    const GCCoord y0 = pt.y;

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    h_renderPixel(x0  , y0+r, brush);
    h_renderPixel(x0  , y0-r, brush);
    h_renderPixel(x0+r, y0, brush);
    h_renderPixel(x0-r, y0, brush);

    while (x<y) {
        if (f >= 0) {
            --y;
            ddF_y += 2;
            f += ddF_y;
        }
        ++x;
        ddF_x += 2;
        f += ddF_x;

        h_renderPixel(x0 + x, y0 + y, brush);
        h_renderPixel(x0 - x, y0 + y, brush);
        h_renderPixel(x0 + x, y0 - y, brush);
        h_renderPixel(x0 - x, y0 - y, brush);
        h_renderPixel(x0 + y, y0 + x, brush);
        h_renderPixel(x0 - y, y0 + x, brush);
        h_renderPixel(x0 + y, y0 - x, brush);
        h_renderPixel(x0 - y, y0 - x, brush);
    }
}

void self_t::h_drawCircleHelper(GCPoint const& pt, GCCoord r, uint8_t cornername, GCBrush const& brush)
{
    const GCCoord x0 = pt.x;
    const GCCoord y0 = pt.y;

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) 
    {
        if (f >= 0) {
            --y;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            h_renderPixel(x0 + x, y0 + y, brush);
            h_renderPixel(x0 + y, y0 + x, brush);
        } 
        if (cornername & 0x2) {
            h_renderPixel(x0 + x, y0 - y, brush);
            h_renderPixel(x0 + y, y0 - x, brush);
        }
        if (cornername & 0x8) {
            h_renderPixel(x0 - y, y0 + x, brush);
            h_renderPixel(x0 - x, y0 + y, brush);
        }
        if (cornername & 0x1) {
            h_renderPixel(x0 - y, y0 - x, brush);
            h_renderPixel(x0 - x, y0 - y, brush);
        }
    }
}

void self_t::h_fillCircle(GCPoint const& pt, GCCoord r, GCBrush const& brush)
{
    h_drawVLine({pt.x, pt.y-r}, 2*r+1, brush);
    h_fillCircleHelper(pt, r, 3, 0, brush);
}

void self_t::h_fillCircleHelper(GCPoint const& pt, GCCoord r, uint8_t cornername, GCCoord delta, GCBrush const& brush)
{
    const GCCoord x0 = pt.x;
    const GCCoord y0 = pt.y;

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x < y)
    {
        const bool changed = (f >= 0);
        if (f >= 0) {
            --y;
            ddF_y += 2;
            f += ddF_y;
        }
        ++x;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1)
            h_drawVLine(x0+x, y0-y, 2*y+1+delta, brush);
        if (cornername & 0x2)
            h_drawVLine(x0-x, y0-y, 2*y+1+delta, brush);

        if (changed) {
        if (cornername & 0x1)
            h_drawVLine(x0+y, y0-x, 2*x+1+delta, brush);
        if (cornername & 0x2)
            h_drawVLine(x0-y, y0-x, 2*x+1+delta, brush);
        }
    }
}

void self_t::h_fillRoundedHelper(GCRect const& rc, bool left, GCBrush const& brush, GCBrush const* brushBg)
{
    constexpr uint8_t offsets[] = 
    {
        1,          // w = 1
        1,2,        // w = 2 
        0,1,3,      // w = 3
        0,1,3,4,
        0,2,3,4,5,
    };

    const uint16_t w = rc.width();
    const uint16_t h = rc.height();

    if (w > 4 || h < w*2) {
        h_fillRect(rc, brush);
    }
    else {
        const uint16_t ptr_offset = (w * w - w) / 2 + w;
        const uint8_t* data = offsets + ptr_offset;

        const int x1 = rc.x1();
        const int x2 = rc.x2();
        const int y0 = rc.y1();

        if (left) {
            for (int x = x2; x >= x1; --x) {
                const auto delta = *data++;
                h_drawVLine(x, y0+delta, (h - delta) - delta, brush);
            }
            if (brushBg)
            {
                for (int x = x2; x >= x1; --x) {
                    const auto delta = *data++;
                    h_drawVLine(x, rc.ry.a, delta, *brushBg);
                    h_drawVLine(x, rc.ry.b-delta, delta, *brushBg);
                }
            }
        }
        else {
            for (int x = x1; x <= x2; ++x) {
                const auto delta = *data++;
                h_drawVLine(x, y0+delta, (h - delta) - delta, brush);
            }
            if (brushBg)
            {
                for (int x = x1; x <= x2; ++x) {
                    const auto delta = *data++;
                    h_drawVLine(x, rc.ry.a, delta, *brushBg);
                    h_drawVLine(x, rc.ry.b-delta, delta, *brushBg);
                }
            }
        }
    }

    // radius = rc.width() + 1;
}

void self_t::h_drawTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3, GCBrush const& brush)
{
    h_drawLine({p1, p2}, brush);
    h_drawLine({p2, p3}, brush);
    h_drawLine({p3, p1}, brush);
}

void self_t::h_fillTriangle(GCPoint const& p0, GCPoint const& p1, GCPoint const& p2, GCBrush const& brush)
{
    int x0 = p0.x;
    int y0 = p0.y;
    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;


    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        std::swap(y0, y1); std::swap(x0, x1);
    }
    if (y1 > y2) {
        std::swap(y2, y1); std::swap(x2, x1);
    }
    if (y0 > y1) {
        std::swap(y0, y1); std::swap(x0, x1);
    }

    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        h_drawHLine(a, y0, b-a+1, brush);
        return;
    }

    int16_t
        dx01 = x1 - x0,
        dy01 = y1 - y0,
        dx02 = x2 - x0,
        dy02 = y2 - y0,
        dx12 = x2 - x1,
        dy12 = y2 - y1;
    int32_t
        sa   = 0,
        sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it

    for(y=y0; y<=last; ++y) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) 
            std::swap(a,b);
        h_drawHLine(a, y, b-a+1, brush);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) 
            std::swap(a,b);
        h_drawHLine(a, y, b-a+1, brush);
    }
}

void self_t::h_drawRoundRect(GCRect const& rc, GCCoord r, GCBrush const& brush)
{
    const int x = rc.x1();
    const int y = rc.y1();
    const int w = rc.width();
    const int h = rc.height();

    // smarter version
    h_drawHLine({x+r  , y    }, w-2*r, brush); // Top
    h_drawHLine({x+r  , y+h-1}, w-2*r, brush); // Bottom
    h_drawVLine({x    , y+r  }, h-2*r, brush); // Left
    h_drawVLine({x+w-1, y+r  }, h-2*r, brush); // Right
    // draw four corners
    h_drawCircleHelper({x+r    , y+r    }, r, 1, brush);
    h_drawCircleHelper({x+w-r-1, y+r    }, r, 2, brush);
    h_drawCircleHelper({x+w-r-1, y+h-r-1}, r, 4, brush);
    h_drawCircleHelper({x+r    , y+h-r-1}, r, 8, brush);
}

void self_t::h_fillRoundRect(GCRect const& rc, GCCoord r, GCBrush const& brush)
{
    const int w = rc.width();
    const int h = rc.height();

    // smarter version
    h_fillRect(GCRect::createSized(rc.x1()+r, rc.y1(), w-2*r, h), brush);

    // draw four corners
    h_fillCircleHelper({rc.x1()+w-r-1, rc.y1()+r}, r, 1, h-2*r-1, brush);
    h_fillCircleHelper({rc.x1()+r    , rc.y1()+r}, r, 2, h-2*r-1, brush);
}


/// todo: improve perfomance, clipping
/// 
void self_t::h_drawBitmap(GCPoint const& pt, const uint8_t *bitmap, uint16_t w, uint16_t h, GCBrush const& brush)
{
    uint16_t byteWidth = (w + 7) / 8; // scanline width in bytes

    //const auto rcImg = clip().intersection(GCRect::createSized(x0,y0,w,h));
    //h = rcImg.height();
    //w = rcImg.width();

    for (int16_t j = 0; j<h; ++j) {
    for (int16_t i = 0; i<w; ++i) {
        const uint8_t b = bitmap[j * byteWidth + i / 8];
        const uint8_t mask = 128 >> (i & 7);
        if (b & mask) {
            h_renderPixel(pt.x+i, pt.y+j, brush);
        }
    }
    }
}


/// todo: improve perfomance, clipping
/// 
void self_t::h_drawXBitmap(GCPoint const& pt, const uint8_t *bitmap,  uint16_t w, uint16_t h, GCBrush const& brush)
{
    uint16_t byteWidth = (w + 7) / 8; // scanline byte size
    
    for(int16_t j=0; j<h; j++) {
    for(int16_t i=0; i<w; i++ ) {
        const uint8_t b = bitmap[j * byteWidth + i / 8];
        const uint8_t mask = 1 << (i % 8);
        if (b & mask) {
            h_renderPixel(pt.x+i, pt.y+j, brush);
        }
        //if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (1 << (i % 8))) {
        //    drawer.drawPixel(x+i, y+j, color, clp);
        //}
    }
    }
}


static uint8_t leadingBits(uint8_t v)
{
    constexpr uint8_t BITS = sizeof(v)*CHAR_BIT;
    constexpr uint8_t mask = 1 << (BITS-1);
    uint8_t r = 0;
    while(v & mask) {
        v <<= 1;
        ++r;
    }
    return r;
}

static uint8_t leadingBits(uint16_t v)
{
    constexpr uint8_t BITS = sizeof(v)*CHAR_BIT;
    constexpr uint16_t mask = 1 << (BITS-1);
    uint8_t r = 0;
    while(v & mask) {
        v <<= 1;
        ++r;
    }
    return r;
}

static uint8_t leadingBits(uint32_t v)
{
    constexpr uint8_t BITS = sizeof(v)*CHAR_BIT;
    constexpr uint32_t mask = 1 << (BITS-1);
    uint8_t r = 0;
    while(v & mask) {
        v <<= 1;
        ++r;
    }
    return r;
}

void self_t::h_drawCharRow(uint8_t line, GCPoint const& pt, GCSize const& scale, uint8_t ts, GCBrush const& brush)
{
    constexpr uint8_t bits = sizeof(line)*8;
    constexpr uint8_t mask = 1 << (bits-1);
    int x = pt.x;

    while (line) {
        
        if(line & mask) {
            auto leading = leadingBits(line);
            h_renderRect(x, pt.y, scale.sx*leading, scale.sy, brush);
            x += scale.sx*leading;
            line <<= leading;
        }
        else {
            x += scale.sx;
            line <<= 1;
        }
        /*
        if(line & mask) { // 1000_0000
            if(line & mask2) {
                h_renderValidRect(x, y, sizeX*2, sizeY, brush);
                x += sizeX;
                line <<= 1;
            }
            else  {
                h_renderValidRect(x, y, sizeX, sizeY, brush);
            }
        }
        x += sizeX;
        line <<= 1;
        */
    }
}

void self_t::h_drawCharRow32(uint32_t line, GCPoint const& pt, GCSize const& scale, uint8_t ts, GCBrush const& brush)
{
    constexpr uint8_t bits = sizeof(line)*8;
    constexpr uint32_t mask = 1 << (bits-1);
    int x = pt.x;

    while (line) {
        if(line & mask) {
            auto leading = leadingBits(line);
            h_renderRect(x, pt.y, scale.sx*leading, scale.sy, brush);
            x += scale.sx*leading;
            line <<= leading;
        }
        else {
            x += scale.sx;
            line <<= 1;
        }
    }
}

/*
static void glyphToBitmap(uint8_t const* glyphBitmap, uint8_t glyphW, uint8_t glyphH, uint32_t* bitmap )
{
    constexpr uint8_t BITS =  sizeof(uint32_t)*CHAR_BIT;
    uint8_t bits = 0, bit = 0;
    for(uint8_t yy=0; yy<glyphH; ++yy) {
        for(uint8_t xx=0; xx<glyphW; ++xx) {
            if(!(bit++ & 7)) { // empty line
                bits = *glyphBitmap++;
            }

            if(bits & 0x80) { // 1000_0000
                *bitmap |= 1;
                //h_renderValidRect(pt.x+(xo+xx)*scale.sx, pt.y+(yo+yy)*scale.sy, scale.sx, scale.sy, brush);
            }
            bits <<= 1;
            *bitmap <<= 1;
        }

        *bitmap <<= (BITS - glyphW - 1);
        ++bitmap;
    }
}
*/

uint8_t self_t::h_drawChar(TextRenderCtx const& ctx, GCPoint const& pt, unsigned char c)
{
    //c -= ctx.font.first;
    //GFXglyph const& glyph = ctx.font.glyph[c];
    
    GFXglyph const& glyph = ctx.font.glyphRef(c);
    uint8_t const* bitmap = ctx.font.bitmap + glyph.bitmapOffset; 

    const uint8_t w  = glyph.width;
    const uint8_t h  = glyph.height;
    const int16_t xo = glyph.xOffset;
    const int16_t yo = glyph.yOffset + ctx.font.baseline();// int16_t(ctx.font.yAdvance*0.8);
    uint8_t bits = 0, bit = 0;

    // Todo: Add character clipping here

    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
    // has typically been used with the 'classic' font to overwrite old
    // screen contents with new data.  This ONLY works because the
    // characters are a uniform size; it's not a sensible thing to do with
    // proportionally-spaced fonts with glyphs of varying sizes (and that
    // may overlap).  To replace previously-drawn text when using a custom
    // font, use the getTextBounds() function to determine the smallest
    // rectangle encompassing a string, erase the area with fillRect(),
    // then draw new text.  This WILL infortunately 'blink' the text, but
    // is unavoidable.  Drawing 'background' pixels will NOT fix this,
    // only creates a new set of problems.  Have an idea to work around
    // this (a canvas object type for MCUs that can afford the RAM and
    // displays supporting setAddrWindow() and pushColors()), but haven't
    // implemented this yet.

    if (ctx.brushBg)
    {
        ConstBitmapView bmpView(bitmap, w, h);

        const uint8_t glyphW = glyph.xAdvance * ctx.scale.sx;
        const uint8_t glyphH = ctx.font.yAdvance * ctx.scale.sy;

        const auto charRc = GCRect::createSized(pt, GCSize(glyphW,glyphH));

        //if (charRc.positive()) 
        {
            const auto pt2 = pt.movedBy(xo, yo);
            GCBrushBitmap brushChar(pt2, ctx.scale, ctx.brush, *ctx.brushBg, bmpView);
            h_renderRect(charRc, brushChar);
        }
    }
    else
    {
        const int16_t startX = pt.x+xo*ctx.scale.sx;
        const int16_t startY = pt.y+yo*ctx.scale.sy;
        for(uint8_t yy=0; yy<h; ++yy) {
            uint32_t line = 0;
            for(uint8_t xx=0; xx<w; ++xx) {
                if(!(bit++ & 7)) { // empty line
                    bits = *bitmap++;
                }

                if(bits & 0x80) { // 1000_0000
                    line |= 1;
                }
                bits <<= 1;
                line <<= 1;
            }

            line <<= (32 - w - 1);
            h_drawCharRow32(line, {startX, startY+yy*ctx.scale.sy}, ctx.scale, ctx.ts, ctx.brush);
        }
    }
    
    /*if (ctx.ts & TS_Underline) 
    {
        const auto top = pt.y+ctx.scale.sy*2;// + font.yAdvance * size_y;
        const auto width = glyph.xAdvance * ctx.scale.sx;
        h_renderRect(GCRect::createSized(pt.x, top, width, ctx.scale.sy), ctx.brush);
    }*/

    return glyph.xAdvance;
}

uint8_t self_t::h_drawChar( GFXfont const& font, GCPoint const& pt, unsigned char c
                          , GCSize const& scale
                          , uint8_t textStyle
                          , GCBrush const& brush
                          , GCBrush const* brushBg)
{
    TextRenderCtx ctx {font, scale, brush, brushBg, textStyle};
    return h_drawChar(ctx, pt, c);

    /*
    c -= font.first;
    GFXglyph const& glyph  = font.glyph[c];
    uint8_t const* bitmap = font.bitmap + glyph.bitmapOffset; 

    const uint8_t w  = glyph.width;
    const uint8_t h  = glyph.height;
    const int16_t xo = glyph.xOffset;
    const int16_t yo = glyph.yOffset + int16_t(font.yAdvance*0.8);//0;//h + glyph.yOffset;// + font.yAdvance;
    uint8_t bits = 0, bit = 0;

    // Todo: Add character clipping here

    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
    // has typically been used with the 'classic' font to overwrite old
    // screen contents with new data.  This ONLY works because the
    // characters are a uniform size; it's not a sensible thing to do with
    // proportionally-spaced fonts with glyphs of varying sizes (and that
    // may overlap).  To replace previously-drawn text when using a custom
    // font, use the getTextBounds() function to determine the smallest
    // rectangle encompassing a string, erase the area with fillRect(),
    // then draw new text.  This WILL infortunately 'blink' the text, but
    // is unavoidable.  Drawing 'background' pixels will NOT fix this,
    // only creates a new set of problems.  Have an idea to work around
    // this (a canvas object type for MCUs that can afford the RAM and
    // displays supporting setAddrWindow() and pushColors()), but haven't
    // implemented this yet.

    if (brushBg)
    {
        ConstBitmapView bmpView(bitmap, w, h);

        const uint8_t glyphW = glyph.xAdvance * scale.sx;
        const uint8_t glyphH = font.yAdvance * scale.sy;

        auto charRc = GCRect::createSized(pt, GCSize(glyphW,glyphH));

        //if (charRc.positive()) 
        {
            const auto pt2 = pt.movedBy(xo, yo);
            GCBrushBitmap brushChar(pt2, scale, brush, *brushBg, bmpView);
            h_renderValidRect(charRc, brushChar);
        }
    }
    else
    {
        if (brushBg) {
            const int sW = glyph.xAdvance * scale.sx;
            const int sH = font.yAdvance * scale.sy;
            h_renderValidRect(GCRect::createSized(pt.x,pt.y,sW,sH), *brushBg);
        }

        const int16_t startX = pt.x+xo*scale.sx;
        const int16_t startY = pt.y+yo*scale.sy;
        for(uint8_t yy=0; yy<h; ++yy) {
            uint32_t line = 0;
            for(uint8_t xx=0; xx<w; ++xx) {
                if(!(bit++ & 7)) { // empty line
                    bits = *bitmap++;
                }

                if(bits & 0x80) { // 1000_0000
                    line |= 1;
                }
                bits <<= 1;
                line <<= 1;
            }

            line <<= (32 - w - 1);
            h_drawCharRow32(line, {startX, startY+yy*scale.sy}, scale, textStyle, brush);
        }
    }
    
    if (textStyle & TS_Underline) 
    {
        const auto top = pt.y+scale.sy*2;// + font.yAdvance * size_y;
        const auto width = glyph.xAdvance * scale.sx;
        h_renderValidRect(GCRect::createSized(pt.x, top, width, scale.sy), brush);
    }
    */
}


// -----------------------------------------------------------


void self_t::h_renderPixelUnsafe(const GCPoint& pt, GCBrush const& brush)
{
    auto pt2 = pt + state().translate;
    m_device.setAddrWindow(pt2);
    m_device.beginData();
    //brush.paint(pt + state().translateBrush, m_device);
    brush.paint(pt, m_device);
}

void self_t::h_renderPixelUnsafe(int x, int y, GCBrush const& brush)
{
    h_renderPixelUnsafe(GCPoint(x,y), brush);
}

void self_t::h_renderRectUnsafe(const GCRect& rc, GCBrush const& brush)
{
    auto rc2 = rc + state().translate;
    m_device.setAddrWindow(rc2);
    m_device.beginData();
    //brush.paint(rc.movedBy(state().translateBrush), m_device);
    brush.paint(rc, m_device);
}

void self_t::h_renderRectUnsafe(int x, int y, int w, int h, GCBrush const& brush)
{
    h_renderRectUnsafe(GCRect::createSized(x, y, w, h), brush);
}

void self_t::h_renderPixel(const GCPoint& pt, GCBrush const& brush)
{
    if (clip().contain(pt))
        h_renderPixelUnsafe(pt, brush);
}

void self_t::h_renderPixel(int x, int y, GCBrush const& brush)
{
    if (clip().contain(x, y))
        h_renderPixelUnsafe(x, y, brush);
}

void self_t::h_renderRect(const GCRect& rect, GCBrush const& brush)
{
    const auto rc = rect.intersection(clip());
    if (!rc.empty())
        h_renderRectUnsafe(rc, brush);
}

void self_t::h_renderRect(int x, int y, int w, int h, GCBrush const& brush)
{
    h_renderRect(GCRect::createSized(x, y, w, h), brush);
}

/*
void self_t::h_renderInvalidRect(const GCRect& rect, GCBrush const& brush)
{
    h_renderValidRect(rect.validSwaped(), brush);
}
*/


// ------------------------------------------------------------
/*
void self_t::renderPixel(const GCPoint& pt, uint16_t color)
{
    if (clip().contain(pt)) {
        //device.beginCommand();
        device.setAddrWindow(pt);
        device.beginData();
        device.fastWrite(color);
        //device.endData();
    }
}

void self_t::renderPixel(int x, int y, uint16_t color)
{
    renderPixel(GCPoint(x, y), color);
}

void self_t::renderPixel(const GCPoint& pt, GCBrush const& brush)
{
    if (clip().contain(pt)) {
        device.setAddrWindow(pt);
        device.beginData();
        brush.paint(pt, device);
    }
}


void self_t::renderRect(const GCRect& rect, uint16_t color)
{
    auto rc = clip().intersection(rect.validSwaped());
    if (rc.positive()) {
        device.setAddrWindow(rc);
        device.beginData();
        auto n = rc.square();
        device.fastWriteN(color, n);
    }
}

void self_t::renderRect(const GCRect& rect, GCBrush const& brush)
{
    auto rc = clip().intersection(rect.validSwaped());
    if (rc.positive()) {
        device.setAddrWindow(rc);
        device.beginData();
        brush.paint(rect, device);
    }
}


void self_t::renderRectColor(const GCRect& pt, void* color)
{
    renderRect(pt, (uintptr_t)color);
}

void self_t::renderRectBrush(const GCRect& pt, void* brush)
{
    renderRect(pt, *(GCBrush const*)brush);
}

*/

