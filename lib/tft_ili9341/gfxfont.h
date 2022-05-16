// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#pragma once
#ifndef file__GFXFONT_H
#define file__GFXFONT_H

#include <cstdint>


/// Font data stored PER GLYPH
struct GFXglyph
{
	uint16_t bitmapOffset;     ///< Pointer into GFXfont->bitmap
	uint8_t  width;            ///< Bitmap dimensions in pixels
    uint8_t  height;           ///< Bitmap dimensions in pixels
	uint8_t  xAdvance;         ///< Distance to advance cursor (x axis)
	int8_t   xOffset;          ///< X dist from cursor pos to UL corner
    int8_t   yOffset;          ///< Y dist from cursor pos to UL corner
};

/// Data stored for FONT AS A WHOLE
struct GFXfont
{ 
	uint8_t  *bitmap;      ///< Glyph bitmaps, concatenated
	GFXglyph *glyph;       ///< Glyph array
	uint8_t   first;       ///< ASCII extents (first char)
    uint8_t   last;        ///< ASCII extents (last char)
	uint8_t   yAdvance;    ///< Newline distance (y axis)

	/// 80% of height
	uint8_t baseline() const { 
		return (yAdvance * 4 + 5/2) / 5;
	}

	GFXglyph& glyphRef(char c) const {
		uint8_t cc = c;
		return (cc >= first && cc <= last) ? glyph[cc-first] : glyph[0];
	}

	uint8_t charWidth(char c) const {
		return glyphRef(c).xAdvance;
	}

	uint8_t emWidth() const {
		return charWidth('M');
	}
};

#endif // file__GFXFONT_H
