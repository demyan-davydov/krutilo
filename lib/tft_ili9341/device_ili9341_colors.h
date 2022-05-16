// -*- coding:utf-8-unix -*em-
/*!
    @file       device_ili9341_color.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-04

    @brief      Color constants for RGB 565 format
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once 
#ifndef file__DEVICE_ILI9441_COLORS_H
#define file__DEVICE_ILI9441_COLORS_H

#include <cstdint>

constexpr uint16_t make_565_color(int r, int g, int b) 
{
    const uint16_t rr = (r * 31 + 127) / 255;
    const uint16_t gg = (g * 63 + 127) / 255;
    const uint16_t bb = (b * 31 + 127) / 255;

    return (rr << 11) | (gg << 5) | bb;
}

enum Color : uint16_t 
{
    Clr_MAROON                      = 0x8000,
    Clr_DARK_RED                    = 0x8800,
    Clr_BROWN                       = 0xA145,
    Clr_FIREBRICK                   = 0xB104,
    Clr_CRIMSON                     = 0xD8A7,
    Clr_RED                         = 0xF800,
    Clr_TOMATO                      = 0xFB09,
    Clr_CORAL                       = 0xFBEA,
    Clr_INDIAN_RED                  = 0xCAEB,
    Clr_LIGHT_CORAL                 = 0xEC10,
    Clr_DARK_SALMON                 = 0xE4AF,
    Clr_SALMON                      = 0xF40E,
    Clr_LIGHT_SALMON                = 0xFD0F,
    Clr_ORANGE_RED                  = 0xFA20,
    Clr_DARK_ORANGE                 = 0xFC60,
    Clr_ORANGE                      = 0xFD20,
    Clr_GOLD                        = 0xFEA0,
    Clr_DARK_GOLDEN_ROD             = 0xB421,
    Clr_GOLDEN_ROD                  = 0xDD24,
    Clr_PALE_GOLDEN_ROD             = 0xEF35,
    Clr_DARK_KHAKI                  = 0xBDAD,
    Clr_KHAKI                       = 0xEF31,
    Clr_OLIVE                       = 0x8400,
    Clr_YELLOW                      = 0xFFE0,
    Clr_YELLOW_GREEN                = 0x9E66,
    Clr_DARK_OLIVE_GREEN            = 0x5346,
    Clr_OLIVE_DRAB                  = 0x6C64,
    Clr_LAWN_GREEN                  = 0x7FC0,
    Clr_CHART_REUSE                 = 0x7FE0,
    Clr_GREEN_YELLOW                = 0xAFE6,
    Clr_DARK_GREEN                  = 0x0320,
    Clr_GREEN                       = 0x07E0,
    Clr_FOREST_GREEN                = 0x2444,
    Clr_LIME                        = 0x07E0,
    Clr_LIME_GREEN                  = 0x3666,
    Clr_LIGHT_GREEN                 = 0x9772,
    Clr_PALE_GREEN                  = 0x97D2,
    Clr_DARK_SEA_GREEN              = 0x8DD1,
    Clr_MEDIUM_SPRING_GREEN         = 0x07D3,
    Clr_SPRING_GREEN                = 0x07EF,
    Clr_SEA_GREEN                   = 0x344B,
    Clr_MEDIUM_AQUA_MARINE          = 0x6675,
    Clr_MEDIUM_SEA_GREEN            = 0x3D8E,
    Clr_LIGHT_SEA_GREEN             = 0x2595,
    Clr_DARK_SLATE_GRAY             = 0x328A,
    Clr_TEAL                        = 0x0410,
    Clr_DARK_CYAN                   = 0x0451,
    Clr_AQUA                        = 0x07FF,
    Clr_CYAN                        = 0x07FF,
    Clr_LIGHT_CYAN                  = 0xDFFF,
    Clr_DARK_TURQUOISE              = 0x0679,
    Clr_TURQUOISE                   = 0x46F9,
    Clr_MEDIUM_TURQUOISE            = 0x4E99,
    Clr_PALE_TURQUOISE              = 0xAF7D,
    Clr_AQUA_MARINE                 = 0x7FFA,
    Clr_POWDER_BLUE                 = 0xAEFC,
    Clr_CADET_BLUE                  = 0x64F3,
    Clr_STEEL_BLUE                  = 0x4C16,
    Clr_CORN_FLOWER_BLUE            = 0x64BD,
    Clr_DEEP_SKY_BLUE               = 0x05FF,
    Clr_DODGER_BLUE                 = 0x249F,
    Clr_LIGHT_BLUE                  = 0xAEBC,
    Clr_SKY_BLUE                    = 0x867D,
    Clr_LIGHT_SKY_BLUE              = 0x867E,
    Clr_MIDNIGHT_BLUE               = 0x18CE,
    Clr_NAVY                        = 0x000F,
    Clr_DARK_BLUE                   = 0x0011,
    Clr_MEDIUM_BLUE                 = 0x0019,
    Clr_BLUE                        = 0x001F,
    Clr_ROYAL_BLUE                  = 0x435B,
    Clr_BLUE_VIOLET                 = 0x897B,
    Clr_INDIGO                      = 0x4810,
    Clr_DARK_SLATE_BLUE             = 0x49F1,
    Clr_SLATE_BLUE                  = 0x6AD9,
    Clr_MEDIUM_SLATE_BLUE           = 0x7B5D,
    Clr_MEDIUM_PURPLE               = 0x939B,
    Clr_DARK_MAGENTA                = 0x8811,
    Clr_DARK_VIOLET                 = 0x901A,
    Clr_DARK_ORCHID                 = 0x9999,
    Clr_MEDIUM_ORCHID               = 0xBABA,
    Clr_PURPLE                      = 0x8010,
    Clr_THISTLE                     = 0xD5FA,
    Clr_PLUM                        = 0xDD1B,
    Clr_VIOLET                      = 0xEC1D,
    Clr_MAGENTA                     = 0xF81F,
    Clr_ORCHID                      = 0xDB9A,
    Clr_MEDIUM_VIOLET_RED           = 0xC0B0,
    Clr_PALE_VIOLET_RED             = 0xDB92,
    Clr_DEEP_PINK                   = 0xF8B2,
    Clr_HOT_PINK                    = 0xFB56,
    Clr_LIGHT_PINK                  = 0xFDB7,
    Clr_PINK                        = 0xFDF9,
    Clr_ANTIQUE_WHITE               = 0xF75A,
    Clr_BEIGE                       = 0xF7BB,
    Clr_BISQUE                      = 0xFF18,
    Clr_BLANCHED_ALMOND             = 0xFF59,
    Clr_WHEAT                       = 0xF6F6,
    Clr_CORN_SILK                   = 0xFFBB,
    Clr_LEMON_CHIFFON               = 0xFFD9,
    Clr_LIGHT_GOLDEN_ROD_YELLOW     = 0xF7DA,
    Clr_LIGHT_YELLOW                = 0xFFEF,
    Clr_SADDLE_BROWN                = 0x8A22,
    Clr_SIENNA                      = 0x9A85,
    Clr_CHOCOLATE                   = 0xD344,
    Clr_PERU                        = 0xCC28,
    Clr_SANDY_BROWN                 = 0xF52C,
    Clr_BURLY_WOOD                  = 0xDDB0,
    Clr_TAN                         = 0xD591,
    Clr_ROSY_BROWN                  = 0xBC71,
    Clr_MOCCASIN                    = 0xFF16,
    Clr_NAVAJO_WHITE                = 0xFEF5,
    Clr_PEACH_PUFF                  = 0xFED6,
    Clr_MISTY_ROSE                  = 0xFF1B,
    Clr_LAVENDER_BLUSH              = 0xFF7E,
    Clr_LINEN                       = 0xF77C,
    Clr_OLD_LACE                    = 0xFFBC,
    Clr_PAPAYA_WHIP                 = 0xFF7A,
    Clr_SEA_SHELL                   = 0xFFBD,
    Clr_MINT_CREAM                  = 0xF7FE,
    Clr_SLATE_GRAY                  = 0x7412,
    Clr_LIGHT_SLATE_GRAY            = 0x7453,
    Clr_LIGHT_STEEL_BLUE            = 0xAE1B,
    Clr_LAVENDER                    = 0xE73E,
    Clr_FLORAL_WHITE                = 0xFFDD,
    Clr_ALICE_BLUE                  = 0xEFBF,
    Clr_GHOST_WHITE                 = 0xF7BF,
    Clr_HONEYDEW                    = 0xEFFD,
    Clr_IVORY                       = 0xFFFD,
    Clr_AZURE                       = 0xEFFF,
    Clr_SNOW                        = 0xFFDE,
    Clr_BLACK                       = 0x0000,
    Clr_DIM_GRAY                    = 0x6B4D,
    //Clr_GRAY                        = 0x8410,
    //Clr_DARK_GRAY                   = 0xAD55,
    Clr_DARK_GRAY                   = 0x39E7,
    Clr_SILVER                      = 0xBDF7,
    Clr_LIGHT_GRAY                  = 0xD69A,
    Clr_GRAY                        = 0x7BEF,  ///< 127, 127, 127
    Clr_GAINSBORO                   = 0xDEDB,
    Clr_WHITE_SMOKE                 = 0xF7BE,
    Clr_WHITE                       = 0xFFFF,

/*
    Clr_BLACK           = 0x0000,  ///<   0,   0,   0
    Clr_WHITE           = 0xFFFF,  ///< 255, 255, 255
    Clr_GRAY            = 0x7BEF,  ///< 127, 127, 127
    Clr_LIGHT_GRAY      = 0xC618,  ///< 198, 195, 198
    Clr_DARK_GRAY       = 0x39E7,  ///<  64,  64,  64

    Clr_NAVY            = 0x000F,  ///<   0,   0, 123

    Clr_DARK_GREEN      = 0x03E0,  ///<   0, 125,   0
    Clr_DARK_CYAN       = 0x03EF,  ///<   0, 125, 123
    Clr_MAROON          = 0x8000,  ///< 123,   0,   0
    Clr_PURPLE          = 0x8010,  ///< 123,   0, 123


    Clr_OLIVE           = 0x7BE0,  ///< 123, 125,   0
    Clr_BLUE            = 0x001F,  ///<   0,   0, 255
    Clr_GREEN           = 0x07E0,  ///<   0, 255,   0
    Clr_CYAN            = 0x07FF,  ///<   0, 255, 255
    Clr_RED             = 0xF800,  ///< 255,   0,   0
    Clr_DARK_RED        = 0x8800,  
    Clr_BROWN           = 0xA145, 
    Clr_FIREBRICK       = 0xB104, 
    Clr_TOMATO          = 0xFB09, 

    Clr_CORAL           = 0xFBEA,
    Clr_INDIAN_RED      = 0xCAEB,
    Clr_LIGHT_CORAL     = 0xEC10,
    Clr_DARK_SALMON     = 0xE4AF,
    Clr_SALMON          = 0xF40E,
    Clr_LIGHT_SALMON    = 0xFD0F,
    Clr_ORANGE_RED      = 0xFA20,
    Clr_DARK_ORANGE     = 0xFC60,
    Clr_MAGENTA         = 0xF81F,  ///< 255,   0, 255
    Clr_LIGHT_YELLOW    = 0xFFEF,  ///< 255, 255, 127
    Clr_YELLOW          = 0xFFE0,  ///< 255, 255,   0
    Clr_YELLOW_GREEN    = 0x9E66,
    Clr_ORANGE          = 0xFD20,  ///< 255, 165,   0
    Clr_GOLD            = 0xFEA0,
    Clr_DARK_GOLDEN_ROD = 0xB421,
    Clr_GOLDEN_ROD      = 0xDD24,
    Clr_PALE_GOLDEN_ROD = 0xEF35,
    Clr_DARK_KHAKI      = 0xBDAD,
    Clr_KHAKI           = 0xEF31,
    Clr_OLIVE           = 0x8400,

    Clr_GREENYELLOW     = 0xAFE5,  ///< 173, 255,  41
    Clr_PINK            = 0xFC18,  ///< 255, 130, 198
*/
};


#endif // file__DEVICE_ILI9441_COLORS_H
