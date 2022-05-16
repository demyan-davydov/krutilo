#include "brush_utils.h"


static uint8_t getR(uint16_t c) { return (c >> 11); }
static uint8_t getG(uint16_t c) { return (c >> 5) & 63; }
static uint8_t getB(uint16_t c) { return (c & 31); }

/*
/// convert number range from [0..255] to [0..31]
static uint8_t conv8to5(uint8_t v) {
    // v/255 = x/31 => x = v*31/255
    return (uint8_t)(((uint16_t)v*(uint16_t)31) / (uint16_t)255);
}

/// convert number range from [0..255] to [0..63]
static uint8_t conv8to6(uint8_t v) {
    // v/255 = x/63 => x = v*63/255
    return (uint8_t)(((uint16_t)v*(uint16_t)63) / (uint16_t)255);
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
static uint16_t color565(uint8_t r, uint8_t g, uint8_t b) 
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
*/

// build 565 color from raw components
static uint16_t build565(uint16_t r, uint16_t g, uint16_t b)
{
    return (r << 11) | (g << 5) | b;
}

/// mix color components
static uint16_t mixComponents(uint16_t c1, uint16_t c2, uint16_t weight1, uint16_t weight2, uint16_t weightSumm) 
{
    // todo: gamma correction? good but slow
    const uint32_t summ = c1 * weight1 + c2 * weight2; 
    return (summ + (weightSumm/2)) / weightSumm;
}

/// mix color components
static uint16_t mixComponents_f(uint16_t c1, uint16_t c2, uint16_t weight1, float weight2, float weightSumm) 
{
    // todo: gamma correction? good but slow
    const float summ = c1 * weight1 + c2 * weight2; 
    return (summ) / weightSumm + 0.5f;
}

static uint16_t mixComponents_fix4(uint16_t c1, uint16_t c2, uint32_t weight1, uint32_t weight2, uint32_t weightSumm) 
{
    // todo: gamma correction? good but slow
    const uint32_t summ = c1 * weight1 + c2 * weight2; 
    return ((summ) + (weightSumm/2)) / weightSumm;
}

uint16_t mix565colors(uint16_t c1, uint16_t c2, uint16_t weight1, uint16_t weight2)
{
    const uint16_t ws = weight1 + weight2; // weights summ

    const uint16_t R = mixComponents(getR(c1), getR(c2), weight1, weight2, ws);
    const uint16_t G = mixComponents(getG(c1), getG(c2), weight1, weight2, ws);
    const uint16_t B = mixComponents(getB(c1), getB(c2), weight1, weight2, ws);

    return build565(R, G, B);
}

uint16_t mix565colors_f(uint16_t c1, uint16_t c2, float weight1, float weight2)
{
    const uint16_t ws = weight1 + weight2; // weights summ

    const uint16_t R = mixComponents_f(getR(c1), getR(c2), weight1, weight2, ws);
    const uint16_t G = mixComponents_f(getG(c1), getG(c2), weight1, weight2, ws);
    const uint16_t B = mixComponents_f(getB(c1), getB(c2), weight1, weight2, ws);

    return build565(R, G, B);
}

uint16_t mix565colors_fix4(uint16_t c1, uint16_t c2, uint32_t weight1, uint32_t weight2)
{
    const uint32_t ws = weight1 + weight2; // weights summ

    const uint16_t R = mixComponents_fix4(getR(c1), getR(c2), weight1, weight2, ws);
    const uint16_t G = mixComponents_fix4(getG(c1), getG(c2), weight1, weight2, ws);
    const uint16_t B = mixComponents_fix4(getB(c1), getB(c2), weight1, weight2, ws);

    return build565(R, G, B);
}

/// mix color components
static uint16_t mixComponents(uint16_t c1, uint16_t c2, float w2) 
{
    return c1 * (1.f - w2) + c2 * w2; 
}

uint16_t mix565colors(uint16_t c1, uint16_t c2, float weight2)
{
    const uint16_t R = mixComponents(getR(c1), getR(c2), weight2);
    const uint16_t G = mixComponents(getG(c1), getG(c2), weight2);
    const uint16_t B = mixComponents(getB(c1), getB(c2), weight2);

    return build565(R, G, B);
}
