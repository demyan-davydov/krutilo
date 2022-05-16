#ifndef file__BURST_SPI_X_H
#define file__BURST_SPI_X_H

#include "BurstSPI.h"

class BurstSpiX : public BurstSPI
{
public:
    BurstSpiX(PinName mosi, PinName miso, PinName sclk) : BurstSPI(mosi, miso, sclk) {};

    int getFormatBits() const { return _bits; }
    int getFormatMode() const { return _mode; }

    void writeFew(int a, int b);
    void writeFew(int a, int b, int c);
    void writeFew(int a, int b, int c, int d);
    void writeFew(int a, int b, int c, int d, int e);
    void writeFew(int a, int b, int c, int d, int e, int f);

};

#endif // file__BURST_SPI_X_H
