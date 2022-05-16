#pragma once
#ifndef file__BITMAP_VIEW_H
#define file__BITMAP_VIEW_H

#include <cstdint> // uint8_t
#include <limits.h> // CHAR_BITS
#include "geo/gc_rect.h"



class ConstBitmapView
{
    uint8_t const* m_data = nullptr;
    uint16_t m_w = 0;
    uint16_t m_h = 0;

public:
    ConstBitmapView(uint8_t const* data_, uint16_t w_, uint16_t h_) 
    : m_data(data_)
    , m_w(w_)
    , m_h(h_)
    {

    }

    void assign(uint8_t const* data_, uint16_t w_, uint16_t h_) 
    {
        m_data = data_;
        m_w = w_;
        m_h = h_;
    }

    bool empty() const { return m_w == 0 || m_h == 0; }

    uint16_t width() const { return m_w; }

    uint16_t height() const { return m_h; }

    //void clear()

    bool pixel(uint16_t x, uint16_t y) const {
        const uint32_t bit = (uint32_t)y * m_w + x;
        const uint32_t byte_n = bit / 8u;
        const uint8_t bit_offset = uint8_t(bit % 8u);
        return m_data[byte_n] & (0x80 >> bit_offset);
    }
};

/*
class BitmapViewConst 
{
public:
    BitmapViewConst(const uint8_t* data, uint16_t width, uint16_t height);
    BitmapViewConst(const uint8_t* data, uint16_t byteStride, uint16_t leftOffset, uint16_t width, uint16_t height);

    bool empty() const { return w_ == 0 || h_ == 0; }
    GCRect rect() const { return GCRect::createSized(0,0,w_,h_); }

    BitmapViewConst intersectedView(GCRect const& clip) const;

    const uint8_t* rowBegin() const { return data_; }
    const uint8_t* rowEnd() const { return data_ + stride_*h_; }
    const uint8_t* rowNext(const uint8_t* currRow) const { return currRow + stride_; }
    const uint8_t* rowPrev(const uint8_t* currRow) const { return currRow - stride_; }
    const uint8_t* row(uint16_t y) const;
    bool bitFromRow(const uint8_t* row, uint16_t x);
    bool bit(uint16_t x, uint16_t y);

protected:
    uint8_t const* data_;
    uint16_t w_, h_;
    const uint16_t offset_; // offset in first byte
    const uint8_t stride_; // row size in bytes
};

class BitmapView : public BitmapViewConst
{
    typedef BitmapViewConst base_t;

public:
    template<int W, int H>
    struct Data {
        enum {
            stride = (W + CHAR_BIT - 1) / CHAR_BIT,
        };
        uint8_t values[stride*H];
    };

    BitmapView(uint8_t* data, uint16_t width, uint16_t height);
    BitmapView(uint8_t* data, uint16_t byteStride, uint16_t leftOffset, uint16_t width, uint16_t height);

    const uint8_t* rowBegin() const { return data_; }
    const uint8_t* rowEnd() const { return data_ + stride_*h_; }

    uint8_t* row(uint16_t y);
    void setBit(uint8_t* row, uint16_t x, bool v);
    void bit(uint16_t x, uint16_t y, bool v);

protected:
    void shiftLeft(uint16_t nBits);

    uint8_t* data() { return const_cast<uint8_t*>(data_); }
};

*/

#endif // file__BITMAP_VIEW_H
