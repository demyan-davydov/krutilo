#include "bitmap_view.h"
#include <limits.h> // CHAR_BITS




/*
BitmapViewConst::BitmapViewConst(const uint8_t* data, uint16_t width, uint16_t height)
: data_(data), w_(width), h_(height), offset_(0)
, stride_((w_ + offset_ + CHAR_BIT - 1) / CHAR_BIT)
{
    
}

BitmapViewConst::BitmapViewConst(const uint8_t* data, uint16_t byteStride, uint16_t leftOffset, uint16_t width, uint16_t height)
: data_(data), w_(width), h_(height), offset_(leftOffset)
, stride_(byteStride)
{
    
}

BitmapViewConst BitmapViewConst::intersectedView(GCRect const& clip) const
{
    GCRect res = rect().intersection(clip);

    if (res.positive()) {
        auto d2 = data_ + stride_*res.y1();
        BitmapViewConst(d2, stride_, offset_ + res.x1(), res.width(), res.height());
    }

    return BitmapViewConst(data_, stride_, 0, 0, 0);
}

const uint8_t* BitmapViewConst::row(uint16_t y) const
{
    return data_ + stride_*y;
}

bool BitmapViewConst::bitFromRow(const uint8_t* row, uint16_t x)
{
    x += offset_;
    auto byte_n = x / CHAR_BIT;
    auto bit_n = CHAR_BIT - (x % CHAR_BIT);
    return row[byte_n] & (1 << bit_n);
}

bool BitmapViewConst::bit(uint16_t x, uint16_t y)
{
    return bitFromRow(row(y), x + offset_);
}





BitmapView::BitmapView(uint8_t* data, uint16_t width, uint16_t height) 
: base_t(data, width, height)
{

}

BitmapView::BitmapView(uint8_t* data, uint16_t byteStride, uint16_t leftOffset, uint16_t width, uint16_t height) 
: base_t(data, byteStride, leftOffset, width, height)
{

}

uint8_t* BitmapView::row(uint16_t y)
{
    return const_cast<uint8_t*>(base_t::row(y));
}

void BitmapView::setBit(uint8_t* rowptr, uint16_t x, bool v) // static
{
    x += offset_;
    auto byte_n = x / CHAR_BIT;
    auto bit_n = CHAR_BIT - (x % CHAR_BIT);

    if (v) {
        rowptr[byte_n] |= ((uint8_t)1 << bit_n); 
    }
    else {
        rowptr[byte_n] &= ~((uint8_t)1 << bit_n);
    }
}

void BitmapView::bit(uint16_t x, uint16_t y, bool v)
{
    setBit(row(y), x, v);
}
*/

