#pragma once
#ifndef file__BIT_STREAM_H
#define file__BIT_STREAM_H

#include <cstdint>
#include <limits.h> // CHAR_BIT

namespace utils 
{

class BitStream
{
    uint8_t const* stream; // bit stream
    const uint8_t bits; // chunk size
    uint8_t pos = 0; // pos inside byte
    uint8_t least; // least bits to complete chunk

public:
    BitStream(uint8_t const* stream_, int8_t bitsPerChunk) 
    : stream(stream_), bits(bitsPerChunk), least(bitsPerChunk) {}
    
    // return true if is last byte
    bool pop(uint8_t& val) 
    {
        val = ((*stream) << pos);
        const uint8_t readed = CHAR_BIT-pos;

        if (least < readed) {
            val &= (0xff << (CHAR_BIT - least));
            pos += least;
            least = bits;
            return true;
        }
        else { // read final part of byte and proceed to next char in stream
            least -= readed;
            ++stream;
            if (least) {
                val |= ((*stream) >> readed);
                // erase last bits
                if (least < pos) {
                    val &= (0xff << (pos-least));
                    pos = least;
                    least = bits;
                    return true;
                }
                else {
                    //val &= (0xff >> (pos-least));
                    least -= pos;
                }
            }
            else {
                pos = 0;
            }
        }

        /*if (least == 0) {
            least = bits;
            return true;
        }*/

        return false;
    }
};


class BitStream16
{
    typedef uint16_t T;

    T const* stream; // bit stream
    const uint8_t bits; // chunk size
    uint8_t pos = 0; // pos inside byte
    uint8_t least; // least bits to complete chunk

public:
    BitStream16(uint8_t const* stream_, int8_t bitsPerChunk) 
    : stream((T const*)(void const*)stream_), bits(bitsPerChunk), least(bitsPerChunk) {}
    
    // return true if is last byte
    bool pop(T& val) 
    {
        constexpr uint8_t TBITS = CHAR_BIT*sizeof(T);
        constexpr T FULL_MASK = (T)-1;

        val = ((*stream) << pos);
        const uint8_t readed = TBITS-pos;

        if (least < readed) {
            val &= (FULL_MASK << (TBITS - least));
            pos += least;
            least = bits;
            return true;
        }
        else { // read final part of byte and proceed to next char in stream
            least -= readed;
            ++stream;
            if (least) {
                val |= ((*stream) >> readed);
                // erase last bits
                if (least < pos) {
                    val &= (FULL_MASK << (pos-least));
                    pos = least;
                    least = bits;
                    return true;
                }
                else {
                    least -= pos;
                }
            }
            else {
                pos = 0;
            }

            //pos = std::min(pos, least);
        }

        return false;
    }
};


class BitStream32
{
    typedef uint32_t T;

    T const* stream; // bit stream
    const uint8_t bits; // chunk size
    uint8_t pos = 0; // pos inside byte
    uint8_t least; // least bits to complete chunk

    /*static uint16_t readBitRange(uint16_t from, uint8_t hiBit, uint8_t cnt) {
        auto lo = hiBit + 1 - cnt;
        from &= (1 << (hiBit+1)) - 1;
        from >> lo;
    }*/

public:
    BitStream32(uint8_t const* stream_, int8_t bitsPerChunk) 
    : stream((T const*)(void const*)stream_), bits(bitsPerChunk), least(bitsPerChunk) {}
    
    // return true if is last byte
    bool pop(T& val) 
    {
        constexpr uint8_t TBITS = CHAR_BIT*sizeof(T);
        constexpr T FULL_MASK = (T)-1;

        val = ((*stream) << pos);
        const uint8_t readed = TBITS-pos;

        if (least < readed) {
            val &= (FULL_MASK << (TBITS - least));
            pos += least;
            least = bits;
            return true;
        }
        else { // read final part of byte and proceed to next char in stream
            least -= readed;
            ++stream;
            if (least) {
                val |= ((*stream) >> readed);
                // erase last bits
                if (least < pos) {
                    val &= (FULL_MASK << (pos-least));
                    pos = least;
                    least = bits;
                    return true;
                }
                else {
                    least -= pos;
                }
            }
            else {
                pos = 0;
            }

            //pos = std::min(pos, least);
        }

        //printf ("  return: least=%d pos=%d\n", least, pos);

        /*if (least == 0) {
            least = bits;
            return true;
        }*/

        return false;
    }
};

} // namespace utils

#endif // file__BIT_STREAM_H

