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
#include <cstdint>
#include "device_ili9341.h"
#include "mbed.h"
#include "PinNames.h"
#include "BurstSPI.h"


#define LCD_SPI_MODE 0x03
#define LCD_SPI_BITS 0x10

// May need to lower this on certain boards
//#define LCD_FREQ 6200000
#define LCD_FREQ 36000000

#define PIN_RST  0x00
#define PIN_SCE  0x01
#define PIN_DC   0x02

#define ILI9341_TFTWIDTH   240      ///< ILI9341 max TFT width
#define ILI9341_TFTHEIGHT  320      ///< ILI9341 max TFT height

#define ILI9341_NOP        0x00     ///< No-op register
#define ILI9341_SWRESET    0x01     ///< Software reset register
#define ILI9341_RDDID      0x04     ///< Read display identification information
#define ILI9341_RDDST      0x09     ///< Read Display Status

#define ILI9341_SLPIN      0x10     ///< Enter Sleep Mode
#define ILI9341_SLPOUT     0x11     ///< Sleep Out
#define ILI9341_PTLON      0x12     ///< Partial Mode ON
#define ILI9341_NORON      0x13     ///< Normal Display Mode ON

#define ILI9341_IDMOFF     0x38     ///< Idle mode off
#define ILI9341_IDMON      0x39     ///< Idle mode on


#define ILI9341_RDMODE     0x0A     ///< Read Display Power Mode
#define ILI9341_RDMADCTL   0x0B     ///< Read Display MADCTL
#define ILI9341_RDPIXFMT   0x0C     ///< Read Display Pixel Format
#define ILI9341_RDIMGFMT   0x0D     ///< Read Display Image Format
#define ILI9341_RDSELFDIAG 0x0F     ///< Read Display Self-Diagnostic Result

#define ILI9341_INVOFF     0x20     ///< Display Inversion OFF
#define ILI9341_INVON      0x21     ///< Display Inversion ON
#define ILI9341_GAMMASET   0x26     ///< Gamma Set
#define ILI9341_DISPOFF    0x28     ///< Display OFF
#define ILI9341_DISPON     0x29     ///< Display ON

#define ILI9341_CASET      0x2A     ///< Column Address Set
#define ILI9341_PASET      0x2B     ///< Page Address Set
#define ILI9341_RAMWR      0x2C     ///< Memory Write
#define ILI9341_RAMRD      0x2E     ///< Memory Read

#define ILI9341_PTLAR      0x30     ///< Partial Area
#define ILI9341_VSCRDEF    0x33     ///< Vertical Scrolling Definition
#define ILI9341_MADCTL     0x36     ///< Memory Access Control
#define ILI9341_VSCRSADD   0x37     ///< Vertical Scrolling Start Address
#define ILI9341_PIXFMT     0x3A     ///< COLMOD: Pixel Format Set

#define ILI9341_FRMCTR1    0xB1     ///< Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2    0xB2     ///< Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3    0xB3     ///< Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_INVCTR     0xB4     ///< Display Inversion Control
#define ILI9341_DFUNCTR    0xB6     ///< Display Function Control

#define ILI9341_PWCTR1     0xC0     ///< Power Control 1
#define ILI9341_PWCTR2     0xC1     ///< Power Control 2
#define ILI9341_PWCTR3     0xC2     ///< Power Control 3
#define ILI9341_PWCTR4     0xC3     ///< Power Control 4
#define ILI9341_PWCTR5     0xC4     ///< Power Control 5
#define ILI9341_VMCTR1     0xC5     ///< VCOM Control 1
#define ILI9341_VMCTR2     0xC7     ///< VCOM Control 2

#define ILI9341_RDID1      0xDA     ///< Read ID 1
#define ILI9341_RDID2      0xDB     ///< Read ID 2
#define ILI9341_RDID3      0xDC     ///< Read ID 3
#define ILI9341_RDID4      0xDD     ///< Read ID 4

#define ILI9341_GMCTRP1    0xE0     ///< Positive Gamma Correction
#define ILI9341_GMCTRN1    0xE1     ///< Negative Gamma Correction
//#define ILI9341_PWCTR6     0xFC


#define DELAY 0x80


/*!
    @brief  Instantiate Adafruit ILI9341 driver with software SPI
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    mosi  SPI MOSI pin #
    @param    sck   SPI Clock pin #
    @param    rst   Reset pin # (optional, pass NC if unused)
    @param    miso  SPI MISO pin # (optional, pass 'NC' if unused)
*/

Device_ILI9341::Device_ILI9341(PinName DC, PinName CS, PinName MOSI, PinName SCK, PinName MISO, PinName RST) 
: m_spi(MOSI, MISO, SCK)
{
    m_spi.format(LCD_SPI_BITS, LCD_SPI_MODE);
    m_spi.frequency(LCD_FREQ);
    //spi.set_dma_usage(DMAUsage::DMA_USAGE_NEVER);

    gpio_init_out(&m_gpioDc, DC);
    gpio_init_out(&m_gpioCs, CS);
    gpio_init_out(&m_gpioRst, RST);

    begin();
}

void Device_ILI9341::begin()
{
    setPin_RST(0);
    setPin_DC(0);
    setPin_CS(0);

    // toggle RST low to reset
    if (gpio_is_connected(&m_gpioRst)) 
    {
        setPin_RST(1); wait_us(5000); //ThisThread::sleep_for(5);
        setPin_RST(0); wait_us(20000);
        setPin_RST(1); wait_us(150000); // 150
    }
    else { // If no hardware reset pin...
        sendCommand(ILI9341_SWRESET); // Engage software reset
        wait_us(100000);
    }

    {
        TempFormatBits fbits(m_spi, 8);

        sendCommand(0xEF);
        sendArg(0x03, 0x80, 0x02);

        sendCommand(0xCF);
        sendArg(0x00, 0xc1, 0x30);

        sendCommand(0xED);
        sendArg(0x64, 0x03, 0x12, 0x81);

        sendCommand(0xE8);
        sendArg(0x85, 0x00, 0x78);

        sendCommand(0xCB);
        sendArg(0x39, 0x2c, 0x00, 0x34, 0x02);

        sendCommand(0xF7);
        sendArg(0x20);

        sendCommand(0xEA);
        sendArg(0x00, 0x00);

        sendCommand(ILI9341_PWCTR1);    //Power control
        sendArg(0x23);   //VRH[5:0]

        sendCommand(ILI9341_PWCTR2);    //Power control
        sendArg(0x10);   //SAP[2:0];BT[3:0]

        sendCommand(ILI9341_VMCTR1);    //VCM control
        sendArg(0x3e, 0x28);

        sendCommand(ILI9341_VMCTR2);    //VCM control2
        sendArg(0x86);  //--

        sendCommand(ILI9341_MADCTL);    // Memory Access Control
        sendArg(0x48);

        sendCommand(ILI9341_VSCRSADD);    //  Vertical scroll zero
        sendArg(0x00);

        sendCommand(ILI9341_VSCRDEF);     // Scroll margin
        sendArg(0x00, 0x00, 0x01, 0x40, 0x00, 0x00); 

        sendCommand(ILI9341_PIXFMT);
        sendArg(0x55);

        sendCommand(ILI9341_FRMCTR1);
        sendArg(0x00, 0x18);

        sendCommand(ILI9341_DFUNCTR);    // Display Function Control
        sendArg(0x08, 0x82, 0x27);

        sendCommand(ILI9341_INVOFF); // inverse display off

        sendCommand(0xF2);    // 3Gamma Function Disable
        sendArg(0x00);

        sendCommand(ILI9341_GAMMASET);    //Gamma curve selected
        sendArg(0x01);

        sendCommand(ILI9341_GMCTRP1);    //Set Gamma
        sendArg(0x0f, 0x31, 0x2b, 0x0c, 0x0e);
        sendArg(0x08, 0x4e, 0xf1, 0x37, 0x07);
        sendArg(0x10, 0x03, 0x0e, 0x09, 0x00);

        sendCommand(ILI9341_GMCTRN1);    //Set Gamma
        sendArg(0x00, 0x0e, 0x14, 0x03, 0x11);
        sendArg(0x07, 0x31, 0xc1, 0x48, 0x08);
        sendArg(0x0f, 0x0c, 0x31, 0x36, 0x0f);

        sendCommand(ILI9341_SLPOUT);    //Exit Sleep
        wait_us(100000);
        
        sendCommand(ILI9341_DISPON);    //Display on
        //wait_us(100000);
    }
}

void Device_ILI9341::sendCommand(uint8_t cmd, uint8_t arg1, uint8_t arg2, uint8_t arg3, uint8_t arg4)
{
    setPin_DC(0);
    setPin_CS(0);
    m_spi.write(cmd);
    setPin_DC(1);
    m_spi.writeFew(arg1, arg2, arg3, arg4);
    setPin_CS(1);
}

void Device_ILI9341::sendCommand(uint8_t c) 
{
    setPin_DC(0);
    setPin_CS(0);
    m_spi.write(c);
    setPin_CS(1);
}

void Device_ILI9341::sendArg(uint8_t a)
{
    setPin_DC(1);
    setPin_CS(0);
    m_spi.write(a);
    setPin_CS(1);
}

void Device_ILI9341::sendArg(uint8_t a, uint8_t b)
{
    setPin_DC(1);
    setPin_CS(0);
    m_spi.writeFew(a,b);
    setPin_CS(1);
}

void Device_ILI9341::sendArg(uint8_t a, uint8_t b, uint8_t c)
{
    setPin_DC(1);
    setPin_CS(0);
    m_spi.writeFew(a,b,c);
    setPin_CS(1);

}

void Device_ILI9341::sendArg(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    setPin_DC(1);
    setPin_CS(0);
    m_spi.writeFew(a,b,c,d);
    setPin_CS(1);
}

void Device_ILI9341::sendArg(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e)
{
    setPin_DC(1);
    setPin_CS(0);
    m_spi.writeFew(a,b,c,d,e);
    setPin_CS(1);
}

void Device_ILI9341::sendArg(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f)
{
    setPin_DC(1);
    setPin_CS(0);
    m_spi.writeFew(a,b,c,d,e,f);
    setPin_CS(1);
}

void Device_ILI9341::beginCommand()
{
    setPin_DC(0); // ??
    setPin_CS(0);

    /*
    Pins[PIN_DC]->write(0);
    Pins[PIN_SCE]->write(0);
    spiwrite(c);
    Pins[PIN_SCE]->write(1);
    */
}

void Device_ILI9341::endCommand()
{
    //setPin_DC(1); // ??
    setPin_CS(0);
}

void Device_ILI9341::beginData()
{
    setPin_DC(1);
    setPin_CS(0);
}

void Device_ILI9341::endData()
{
    setPin_CS(1);
}

void Device_ILI9341::setAddrWindow(const GCRect& rc) 
{
    /// This small optimisation makes drawing of lines/circles much faster
    if (m_lastWnd != rc)
    {
        TempFormatBits fbits(m_spi, 8);

        if (m_lastWnd.rx != rc.rx) {
            sendCommand(ILI9341_CASET, rc.x1() >> 8, rc.x1() & 0xff, rc.x2() >> 8, rc.x2() & 0xff);
            m_lastWnd.rx = rc.rx;
        }

        if (m_lastWnd.ry != rc.ry) {
            sendCommand(ILI9341_PASET, rc.y1() >> 8, rc.y1() & 0xff, rc.y2() >> 8, rc.y2() & 0xff);
            m_lastWnd.ry = rc.ry;
        }

        sendCommand(ILI9341_RAMWR); // write to RAM

        // current: DC=0, CS=1
        // prepare for data transfer
        //setPin_DC(1);
        //setPin_CS(0);
    }
}

void Device_ILI9341::setAddrWindow(const GCPoint& pt)
{
    setAddrWindow(GCRect(pt));
}


#define MADCTL_MY  0x80  ///< Bottom to top
#define MADCTL_MX  0x40  ///< Right to left
#define MADCTL_MV  0x20  ///< Reverse Mode
#define MADCTL_ML  0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00  ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08  ///< Blue-Green-Red pixel order
#define MADCTL_MH  0x04  ///< LCD refresh right to left


void Device_ILI9341::invertDisplay(bool i) 
{
    TempFormatBits fbits(m_spi, 8);
    sendCommand(i ? ILI9341_INVON : ILI9341_INVOFF);
}

void Device_ILI9341::scrollTo(uint16_t y)
{
    TempFormatBits fbits(m_spi, 8);
    sendCommand(ILI9341_VSCRSADD);
    sendArg(y >> 8, y & 0xff);
    m_scroll = y;
}

uint16_t Device_ILI9341::scroll() const 
{
    return m_scroll;
}

void Device_ILI9341::scrollMargins(uint16_t top, uint16_t bottom)
{
    TempFormatBits fbits(m_spi, 8);
    if (top + bottom <= ILI9341_TFTHEIGHT) {
        uint16_t middle = ILI9341_TFTHEIGHT - (top + bottom);
        sendCommand(ILI9341_VSCRDEF);
        sendArg(top>>8, top&0xff, middle>>8, middle&0xff, bottom>>8, bottom&0xff); 
    }
}

void Device_ILI9341::sleep(bool s)
{
	//beginTransaction();
	
    TempFormatBits fbits(m_spi, 8);
    sendCommand(s ? ILI9341_SLPIN : ILI9341_SLPOUT);
	//endTransaction();
	//delay(120);
}

void Device_ILI9341::idle(bool i)
{
	//beginTransaction();
	
    TempFormatBits fbits(m_spi, 8);
    sendCommand(i ? ILI9341_IDMON : ILI9341_IDMOFF);
	//endTransaction();
	//delay(120);
}

/*\
// puts display in/out of sleep mode
void ILI9341_due::sleep(boolean s)
{
	beginTransaction();
	writecommand_last(s ? ILI9341_SLPIN : ILI9341_SLPOUT);
	endTransaction();
	delay(120);
}

void ILI9341_due::idle(boolean i) {
	beginTransaction();
	writecommand_last(i ? ILI9341_IDMON : ILI9341_IDMOFF);
	endTransaction();
}


void ILI9341_due::setPowerLevel(pwrLevel p)
{
	switch (p)
	{
	case pwrLevelNormal:
		if (_isIdle) { idle(false); _isIdle = false; }
		if (_isInSleep) { sleep(false); _isInSleep = false; }
		break;
	case pwrLevelIdle:
		if (!_isIdle) { idle(true); _isIdle = true; }
		if (_isInSleep) { sleep(false); _isInSleep = false; }
		break;
	case pwrLevelSleep:
		if (!_isInSleep) { sleep(true); _isInSleep = true; }
		if (_isIdle) { idle(false); _isIdle = false; }
		break;
	}
}

*/


void Device_ILI9341::orientation(Orientation m) 
{
    TempFormatBits fbits(m_spi, 8);
    
    switch (m) 
    {
    case Orient_Normal: // 0
        sendCommand(ILI9341_MADCTL);
        sendArg(MADCTL_MX | MADCTL_BGR);
      break;
    case Orient_90CW: // 1
        sendCommand(ILI9341_MADCTL);
        sendArg(MADCTL_MV | MADCTL_BGR);
      break;
    case Orient_180CW: // 2
        sendCommand(ILI9341_MADCTL);
        sendArg(MADCTL_MY | MADCTL_BGR);
      break;
    case Orient_270CW: // 3
        sendCommand(ILI9341_MADCTL);
        sendArg(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      break;
    }
}

int Device_ILI9341::hardwareWidth() // static
{
    return ILI9341_TFTWIDTH;
}

int Device_ILI9341::hardwareHeight() // static
{
    return ILI9341_TFTHEIGHT;
}

GCSize Device_ILI9341::hardwareSize() // static
{
    return GCSize{ hardwareWidth(), hardwareHeight() };
}

bool Device_ILI9341::isVertical(Orientation o) // static
{
    return o == Orient_Normal || o == Orient_180CW;
}

GCSize Device_ILI9341::screenSize(Orientation o) // static
{
    if (isVertical(o)) {
        return GCSize(hardwareWidth(), hardwareHeight());
    } 
    else {
        return GCSize(hardwareHeight(), hardwareWidth());
    }
}

GCRect Device_ILI9341::screenRect(Orientation o) // static
{
    if (isVertical(o)) {
        return GCRect::createSized(0, 0, hardwareWidth(), hardwareHeight());
    } 
    else {
        return GCRect::createSized(0, 0, hardwareHeight(), hardwareWidth());
    }
}



// ----------------------------------------------------------------------------
// --- Read data from device (not tested)
// ----------------------------------------------------------------------------

uint8_t Device_ILI9341::spiread() 
{
    //spi.clearRX();
    return m_spi.write(0);
}

uint8_t Device_ILI9341::readdata() 
{
    setPin_DC(1);
    setPin_CS(0);
    uint8_t r = (uint8_t)m_spi.write(0);// spiread();
    setPin_CS(1);
    return r;
}


uint8_t Device_ILI9341::readcommand8(uint8_t c, uint8_t index) 
{
    TempFormatBits fbits(m_spi, 8);

    setPin_DC(0);
    setPin_CS(0);

    m_spi.write(0xD9);  // woo sekret command?
    
    setPin_DC(1);
    m_spi.write(0x10 + index);
    setPin_CS(1);

    setPin_DC(0);
    setPin_CS(0);
    m_spi.write(c);
    setPin_DC(1);
    uint8_t r = (uint8_t)m_spi.write(0); // spiread();
    setPin_CS(1);

    return r;
}



// ----------------------------------------------------------------------------
// --- Pin control
// ----------------------------------------------------------------------------

void Device_ILI9341::setPin_RST(int v) 
{
    if (gpio_is_connected(&m_gpioRst))
        gpio_write(&m_gpioRst, v);
}

void Device_ILI9341::setPin_CS(int v) // sce
{
    gpio_write(&m_gpioCs, v);
}

void Device_ILI9341::setPin_DC(int v) 
{
    gpio_write(&m_gpioDc, v);
}


/// experimental

void Device_ILI9341::setPin_CS_1()
{
    gpio_write(&m_gpioCs, 1);
}

void Device_ILI9341::setPin_CS_0()
{
    gpio_write(&m_gpioCs, 0);
}

void Device_ILI9341::setPin_DC_1()
{
    gpio_write(&m_gpioDc, 1);
}

void Device_ILI9341::setPin_DC_0()
{
    gpio_write(&m_gpioDc, 0);
}


void setPin_1(gpio_t* obj) // static
{
#if (defined(TARGET_STM32F4) || defined(TARGET_STM32F103C8))
    *obj->reg_set = obj->mask;
#else
    gpio_write(&g, 1);
#endif
}

void setPin_0(gpio_t* obj) // static
{
#if (defined(TARGET_STM32F4) || defined(TARGET_STM32F103C8))
#   ifdef GPIO_IP_WITHOUT_BRR
        *obj->reg_clr = obj->mask << 16;
#   else
        *obj->reg_clr = obj->mask;
#   endif
#else
    gpio_write(&g, 1);
#endif
}


// ----------------------------------------------------------------------------
// --- Write data to device
// ----------------------------------------------------------------------------


void Device_ILI9341::fastWrite(uint16_t color)
{
    m_spi.fastWrite(color);
}

void Device_ILI9341::fastWriteN(uint16_t color, uint32_t count)
{
    m_spi.fastWriteN(color, count);
/*    
    for (uint32_t i = 0; i < count; ++i) {
        spi.fastWrite(color);
    }

    // Duff device really works!!! 
    // Tested on STM32F103C8 with toolchain-gccarmnoneeabi 1.70201.0
    // it takes about 6722 ms / 6146 ms = 1.094 => ~9% of speed

    if (!count) return;
    uint32_t n = (count + 7) / 8;
    switch (count % 8) {
    case 0: do { spi.fastWrite(color);
    case 7:      spi.fastWrite(color);
    case 6:      spi.fastWrite(color);
    case 5:      spi.fastWrite(color);
    case 4:      spi.fastWrite(color);
    case 3:      spi.fastWrite(color);
    case 2:      spi.fastWrite(color);
    case 1:      spi.fastWrite(color);
               } while (--n > 0);
    }
    */
}

void Device_ILI9341::clearRx()
{
    m_spi.clearRX();
}



// ----------------------------------------------------------------------------
// --- TempFormatBits
// ----------------------------------------------------------------------------

Device_ILI9341::TempFormatBits::TempFormatBits(BurstSpiX& spi, int formatBits) : m_spi(spi)
{
    //auto& spi = driver_.m_spi;
    m_prevFormatBits = m_spi.getFormatBits();
    if (m_prevFormatBits != formatBits) {
        m_spi.format(formatBits, spi.getFormatMode());
    }
}

Device_ILI9341::TempFormatBits::~TempFormatBits()
{
    //auto& spi = driver_.m_spi;
    if (m_prevFormatBits != m_spi.getFormatBits()) {
        m_spi.format(m_prevFormatBits, m_spi.getFormatMode());
    }
}


