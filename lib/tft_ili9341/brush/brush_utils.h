#pragma once
#ifndef file__BRUSH_UTILS_H
#define file__BRUSH_UTILS_H

#include <cstdint> // uint16_t

uint16_t mix565colors(uint16_t c1, uint16_t c2, uint16_t weight1, uint16_t weight2);

uint16_t mix565colors_f(uint16_t c1, uint16_t c2, float weight1, float weight2);

uint16_t mix565colors_fix4(uint16_t c1, uint16_t c2, uint32_t weight1, uint32_t weight2);

uint16_t mix565colors(uint16_t c1, uint16_t c2, float weight2);

#endif // file__BRUSH_UTILS_H