#if defined(TARGET_LPC1549)
#include "BurstSPI.h"

void BurstSPI::fastWrite(int data) {
    while (!(_spi.spi->STAT & (1 << 1)));

    _spi.spi->TXDAT = (data & 0xffff);
    // end of transfer and receive ignore flag
    _spi.spi->TXCTL |= ((1 << 20) | (1 << 22));
}

void BurstSPI::fastWriteN(uint16_t data, uint32_t count)
{
    const uint32_t data32 = data;
    for (uint32_t i = 0; i < count; ++i) {
        while (!(_spi.spi->STAT & (1 << 1)));

        _spi.spi->TXDAT = data32;
        // end of transfer and receive ignore flag
        _spi.spi->TXCTL |= ((1 << 20) | (1 << 22));
    }
}

void BurstSPI::clearRX( void ) {
    //We already set receive ignore flag, so this function is never run.
}

#endif