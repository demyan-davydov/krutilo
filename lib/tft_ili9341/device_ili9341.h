/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
 /*
    Ported to mbed by James Kidd
 */

#ifndef file__DEVICE_ILI9441_H
#define file__DEVICE_ILI9441_H

#include <cstdint>
#include "mbed.h"
#include "burst_spi_x.h"
#include "gc_size.h"
#include "gc_rect.h"


// Color definitions
/*#define ILI9341_BLACK   0x0000
#define ILI9341_BLUE    0x001F
#define ILI9341_RED     0xF800
#define ILI9341_GREEN   0x07E0
#define ILI9341_CYAN    0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW  0xFFE0
#define ILI9341_WHITE   0xFFFF
*/

class Device_ILI9341 : public NonCopyable<Device_ILI9341>
{
    typedef Device_ILI9341 self_t;

    // temporary install SPI transfer format (spi.format()), restore previous in destructor
    //struct TempFormatBits;
    //friend struct TempFormatBits;
    
    struct TempFormatBits {
        TempFormatBits(BurstSpiX& spi, int formatBits);
        ~TempFormatBits();
    private: 
        BurstSpiX& m_spi;
        int m_prevFormatBits;
    };

public:
    typedef uint16_t TColor;

    enum Orientation : uint8_t
    {
        Orient_Normal = 0,
        Orient_90CW  = 1,
        Orient_180CW = 2,
        Orient_270CW = 3,
    };

public:
    Device_ILI9341(PinName DC, PinName CS, PinName MOSI, PinName SCK, PinName MISO=NC, PinName RST=NC);

    /// final device configuration
private:
    void begin();

public:
    /// Invert display colors
    void invertDisplay(bool invert);

    /// Set current scroll value
    void scrollTo(uint16_t y);

    /// Get current scroll value
    uint16_t scroll() const;

    /// set scroll margins
    void scrollMargins(uint16_t top, uint16_t bottom);

    /// set sleep mode
    void sleep(bool s);

    /// set idle mode
    void idle(bool i);


    void beginCommand();
    void endCommand();

    /// call immediately after setAddrWindow(), before data transfer
    void beginData();
    void endData();


    /// set output window, call it before call fastWrite()
    /// specified rectangle must be entire screenRect()
    void setAddrWindow(const GCRect& rc);

    /// set output window, call it before call fastWrite()
    /// specified rectangle must be entire screenRect()
    //void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

    /// set output window for _one_ pixel, call it before call fastWrite()
    /// point coordinate must be entire screenRect()
    void setAddrWindow(const GCPoint& rc);

    /// set output window for _one_ pixel, call it before call fastWrite()
    /// point coordinate must be entire screenRect()
    //void setAddrWindow(uint16_t x, uint16_t y);


    /// set screen orientation
    void orientation(Orientation orient);

    /// hardware depended screen width
    static int hardwareWidth();

    /// hardware depended screen height
    static int hardwareHeight();

    /// hardware depended screen size
    static GCSize hardwareSize();

    /// hardware depended screen size
    static GCRect hardwareRect();

    /// is vertical orientation
    /// height is bigger then width
    static bool isVertical(Orientation o);

    /// screen rectangle depends to rotation
    static GCSize screenSize(Orientation o);

    /// screen rectangle depends to rotation
    static GCRect screenRect(Orientation o);

private:
    /// For these methods (sendXXX) you must set SPI data format to 8-bits
    /// use TempFormatBits for this purpose

    void sendCommand(uint8_t cmd, uint8_t arg1, uint8_t arg2, uint8_t arg3, uint8_t arg4);
    
    void sendCommand(uint8_t c);
    void sendArg(uint8_t a);
    void sendArg(uint8_t a, uint8_t b);
    void sendArg(uint8_t a, uint8_t b, uint8_t c);
    void sendArg(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
    void sendArg(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e);
    void sendArg(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f);

    /// read byte fron device
    uint8_t spiread();
    
    /// 
    uint8_t readdata();
    
    /// 
    uint8_t readcommand8(uint8_t reg, uint8_t index = 0);

    void setPin_RST(int v);
    void setPin_CS(int v); // sce
    void setPin_DC(int v);
    void setPin_CS_1(); // sce
    void setPin_CS_0(); // sce
    void setPin_DC_1();
    void setPin_DC_0();

public:
    /// send pixel to device
    void fastWrite(uint16_t color);

    /// send few pixels to device
    void fastWriteN(uint16_t color, uint32_t count);

    /// clear recieve buffer after call series of fastWrite() methods
    void clearRx();

private:
    BurstSpiX m_spi;
    gpio_t m_gpioDc;  // todo: use PortOut for DC & CS
    gpio_t m_gpioCs;
    gpio_t m_gpioRst;

    uint16_t m_scroll = 0;
    GCRect m_lastWnd = GCRect(-1,-1); /// last fill window, used to optimize setAddrWindow() calls

    //Orientation _orientation = Orient_Normal; // temp
};

#endif // file__DEVICE_ILI9441_H
