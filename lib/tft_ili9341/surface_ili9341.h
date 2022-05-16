#ifndef file__SURFACE_ILI9441_H
#define file__SURFACE_ILI9441_H

//#include "mbed.h"
#include "device_ili9341.h"
#include "device_ili9341_colors.h"
#include "gfxfont.h"
#include "geo/gc_rect.h"
#include "geo/gc_point.h"
#include "geo/gc_line.h"
#include "geo/gc_extents2d.h"
#include "geo/anchor.h"
#include "brush/brush.h"

#include <memory> // unique_ptr



class Surface_ILI9341
{
    Surface_ILI9341(Surface_ILI9341 const&) = delete;
    Surface_ILI9341& operator = (Surface_ILI9341 const&) = delete;

    enum TextStyle : uint8_t
    {
        TS_Italic = 1,
        TS_Underline = 2,
        TS_Strike = 4,
    };

public:
    typedef Surface_ILI9341 self_t;
    typedef Device_ILI9341 TDevice;
    typedef typename TDevice::TColor TColor;
    typedef typename TDevice::Orientation Orientation;
    //using typename Device_ILI9341::Orientation;

    /// c'tor
    Surface_ILI9341(Device_ILI9341& device);

    /// set display inversion
    /// todo: remove, keep on device 
    void invertDisplay(bool invert);

    /// set display orientation
    void orientation(Orientation orient);

    /// get display orientation
    Orientation orientation() const;

    /// screen rectangle depends to rotation
    GCSize screenSize() const;

    /// screen rectangle depends to rotation
    GCRect screenRect() const;

    /*
    /// set screen scroll
    void scrollTo(uint16_t y);
    
    /// get screen scroll
    uint16_t scroll() const;
    */

    /// Reset clip to screen size
    void resetClip();

    /// set new clip
    void clip(const GCRect& rc);

    /// get current clip rectangle
    GCRect const& clip() const;

    /// set coordinate translation
    void translate(GCPoint const& pt);

    /// change coordinate translation
    void translateBy(GCPoint const& pt);

    /// get coordiante translation
    GCPoint translate() const;

    /// set coordinate translation for brush
    void translateBrush(GCPoint const& pt);

    /// change coordinate translation for brush
    void translateBrushBy(GCPoint const& pt);

    /// get coordiante translation for brush
    GCPoint translateBrush() const;

public:
    void fillScreen(uint16_t color);

    void drawRect(GCRect const& rc, GCBrush const& brush);
    void drawRect(GCRect const& rc);
    void fillRect(GCRect const& rc, GCBrush const& brush);
    void fillRect(GCRect const& rc);

    void fillExtentsInner(GCRect const& rc, GCExtents2d const& ext, GCBrush const& brush);

    void fillExtents(GCRect const& rc, GCExtents2d const& ext, GCBrush const& brush);

    void fillExtentsRound(GCRect const& rc, int r, GCExtents2d const& ext, GCBrush const& brush, GCBrush const* brushBg = 0);


    /// draw pixel
    //void pixel(int16_t x, int16_t y);
    
    /// draw line
    void drawLine(GCLine const& line, GCBrush const& brush);
    void drawLine(GCLine const& line);
    void drawLine(GCPoint const& a, GCPoint const& b);
    void drawLine(GCCoord x0, GCCoord y0, GCCoord x1, GCCoord y1);

    void drawVLine(GCPoint const& pt, GCCoord h, GCBrush const& brush);
    void drawVLine(GCPoint const& pt, GCCoord h);
    void drawVLine(GCCoord x, GCCoord y, GCCoord h);

    void drawHLine(GCPoint const& pt, GCCoord w, GCBrush const& brush);
    void drawHLine(GCPoint const& pt, GCCoord w);
    void drawHLine(GCCoord x, GCCoord y, GCCoord w);

    void drawCircle(GCPoint const& pt, GCCoord r, GCBrush const& brush);
    void drawCircle(GCPoint const& pt, GCCoord r);
    void drawCircle(GCCoord x0, GCCoord y0, GCCoord r);
    void fillCircle(GCPoint const& pt, GCCoord r, GCBrush const& brush);
    void fillCircle(GCPoint const& pt, GCCoord r);
    void fillCircle(GCCoord x0, GCCoord y0, GCCoord r);

    void drawTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3, GCBrush const& brush);
    void drawTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3);
    void drawTriangle(GCCoord x0, GCCoord y0, GCCoord x1, GCCoord y1, GCCoord x2, GCCoord y2);
    
    void fillTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3, GCBrush const& brush);
    void fillTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3);
    void fillTriangle(GCCoord x0, GCCoord y0, GCCoord x1, GCCoord y1, GCCoord x2, GCCoord y2);

    void drawRoundRect(GCRect const& rc, GCCoord radius, GCBrush const& brush);
    void drawRoundRect(GCRect const& rc, GCCoord radius);
    void fillRoundRect(GCRect const& rc, GCCoord radius, GCBrush const& brush);
    void fillRoundRect(GCRect const& rc, GCCoord radius);
    
    void drawBitmap(GCPoint const& pt, const uint8_t *bitmap, uint16_t w, uint16_t h);
    //void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
    void drawXBitmap(GCPoint const& pt, const uint8_t *bitmap,  uint16_t w, uint16_t h);

    //GCSize drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
    /*void drawTextCursor(int16_t x, int16_t y, TColor color);
    */

    /// todo: remove
    //void drawChar(char c, GCPoint const& pt);

    /// 
    uint8_t drawChar(char c, GCPoint const& pt, GFXfont const& font, GCBrush const& brushFg, GCBrush const* brushBg);

    /// measure single char
    /// return {0,0} if no selected font
    uint8_t measureCharWidth(GFXfont const& font, char c) const { return font.glyphRef(c).xAdvance; }

    /// measure single char
    /// return {0,0} if no selected font
    GCSize measureChar(GFXfont const& font, char c) const;

    /// measure multi line text
    /// return {0,0} if no selected font
    //GCSize measureText(const char* str, uint16_t strLen, int maxWidth, int startPos, int tabSpaces = 4) const;

    /// measure single line text
    /// return {0,0} if no selected font
    GCSize measureTextLine(GFXfont const& font, const char* str, uint16_t strLen) const;

    /// measure single line text
    /// return {0,0} if no selected font
    GCSize measureTextLine(GFXfont const& font, const char* str) const;


    /// draw multi line text
    /// return continue point
    GCPoint drawTextMulti(const char* str, uint16_t strLen, GCRect const& rc, GCPoint const& start, int tabSpaces = 4);
    
    /// draw single line text
    /// return continue point
    GCPoint drawTextSingle(const char* str, uint16_t len, GCPoint const& start, GFXfont const& font, GCBrush const& brushFg, GCBrush const* brushBg);

    /// draw single line text
    /// return continue point
    GCPoint drawTextSingle(const char* str, GCPoint const& start, GFXfont const& font, GCBrush const& brushFg, GCBrush const* brushBg);

    /// draw single line text
    /// return continue point
    GCPoint drawTextSingle(const char* str, uint16_t strLen, GCPoint const& start);

    /// draw single line text
    /// return continue point
    GCPoint drawTextSingle(const char* str, uint16_t len, GCRect outerRc, Anchor innerAnchor, GCPoint textOffset, 
                           GFXfont const& font, GCBrush const& brushFg, GCBrush const* brushBg);


    self_t& selectPen(GCBrush const& brush);
    self_t& selectBrush(GCBrush const& brush);
    self_t& selectTextColor(GCBrush const& brush);
    self_t& selectTextBgColor(GCBrush const& brush);

    self_t& selectFont(GFXfont const& font);
    self_t& selectTextStyle(bool italic, bool underline);
    self_t& selectTextSize(uint8_t sz);


    // Begin helper functions
protected:
    void h_drawLine(GCLine const& line, GCBrush const& brush);
    void h_fillRect(GCRect const& rc, GCBrush const& brush);
    
    void h_drawHLine(GCPoint const& pt, GCCoord w, GCBrush const& brush);
    void h_drawHLine(GCCoord x, GCCoord y, GCCoord w, GCBrush const& brush);
    void h_drawVLine(GCPoint const& pt, GCCoord h, GCBrush const& brush);
    void h_drawVLine(GCCoord x, GCCoord y, GCCoord h, GCBrush const& brush);

    void h_drawCircle(GCPoint const& pt, GCCoord r, GCBrush const& brush);
    void h_drawCircleHelper(GCPoint const& pt, GCCoord r, uint8_t cornername, GCBrush const& brush);

    void h_fillCircle(GCPoint const& pt, GCCoord r, GCBrush const& brush);
    void h_fillCircleHelper(GCPoint const& pt, GCCoord r, uint8_t cornername, GCCoord delta, GCBrush const& brush);
    void h_fillRoundedHelper(GCRect const& rc, bool left, GCBrush const& brush, GCBrush const* brushBg);

    void h_drawTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3, GCBrush const& brush);
    void h_fillTriangle(GCPoint const& p1, GCPoint const& p2, GCPoint const& p3, GCBrush const& brush);
    
    void h_drawRoundRect(GCRect const& rc, GCCoord radius, GCBrush const& brush);
    void h_fillRoundRect(GCRect const& rc, GCCoord radius, GCBrush const& brush);

    void h_drawBitmap(GCPoint const& pt, const uint8_t *bitmap, uint16_t w, uint16_t h, GCBrush const& brush);
    void h_drawXBitmap(GCPoint const& pt, const uint8_t *bitmap, uint16_t w, uint16_t h, GCBrush const& brush);

    /// draw one row from char 
    void h_drawCharRow(uint8_t line, GCPoint const& pt, GCSize const& scale, uint8_t ts, GCBrush const& brush);
    void h_drawCharRow32(uint32_t line, GCPoint const& pt, GCSize const& scale, uint8_t ts, GCBrush const& brush);


    struct TextRenderCtx
    {
        GFXfont const& font;
        GCSize const& scale;
        GCBrush const& brush; 
        GCBrush const* brushBg;
        uint8_t ts;
    };

    /// @return char width
    uint8_t h_drawChar(TextRenderCtx const& ctx, GCPoint const& pt, unsigned char c);
    
    /// @return char width
    uint8_t h_drawChar(GFXfont const& font, GCPoint const& pt, unsigned char c, GCSize const& scale, uint8_t ts, GCBrush const& brush, GCBrush const* brushBg);


    // Direct write tp device

    /// set pixel on device, not validate clipping
    void h_renderPixelUnsafe(const GCPoint& pt, GCBrush const& brush);

    /// set pixel on device, not validate clipping
    void h_renderPixelUnsafe(int x, int y, GCBrush const& brush);
    
    /// fill rect on device, not validate rectangle & clipping
    void h_renderRectUnsafe(const GCRect& rc, GCBrush const& brush);

    /// fill rect on device, not validate rectangle & clipping
    void h_renderRectUnsafe(int x, int y, int w, int h, GCBrush const& brush);

    /// set pixel on device if is in clipping area
    void h_renderPixel(const GCPoint& pt, GCBrush const& brush);

    /// set pixel on device if is in clipping area
    void h_renderPixel(int x, int y, GCBrush const& brush);

    /// fill rect on device, suppose 'rc' is valid rect, check clipping
    void h_renderRect(const GCRect& rc, GCBrush const& brush);

    /// fill rect on device, suppose 'rc' is valid rect, check clipping
    void h_renderRect(int x, int y, int w, int h, GCBrush const& brush);

    /// fill rect on device, src may be invalid rect, check clipping
    //void h_renderInvalidRect(const GCRect& rc, GCBrush const& brush);

    struct Text
    {
        GFXfont const* font; ///< selected font (weak reference)
        uint8_t textStyle = 0; /// combination of TextStyle flags
        GCSize textScale = GCSize(1, 1); /// todo: remove?
    };

    /// Graphic context state
    struct State
    {
        std::unique_ptr<GCBrush> brushFg; /// foreground brush (lines, borders, etc)
        std::unique_ptr<GCBrush> brushBg; /// background brush (fills)
        std::unique_ptr<GCBrush> brushTextFg; /// foreground brush (text color)
        std::unique_ptr<GCBrush> brushTextBg; /// background brush (text background)

        GCRect clip {0,0,0,0}; ///< clip rectangle
        Orientation orientation = Orientation::Orient_Normal; ///< screen orientation
        
        /// todo: remove from state
        //uint16_t scrollY = 0; ///< current scroll pos [0..screenHeight)
        
        GCPoint translate = {0,0};
        GCPoint translatePen = {0,0};
        GCPoint translateBrush = {0,0};

        GFXfont const* font; ///< selected font (weak reference)
        uint8_t textStyle = 0; /// combination of TextStyle flags
        GCSize textScale = GCSize(1, 1); /// todo: remove?
    };

    /// return current context state
    State& state() { return m_state; }

    /// return current context state (constant reference)
    State const& state() const { return m_state; }

private:
    Device_ILI9341& m_device;
    State m_state;
};

#endif // file__SURFACE_ILI9441_H
