#if defined(TARGET_STM32F103C8)
#include "BurstSPI.h"

void BurstSPI::fastWrite(int data) {
    SPI_TypeDef *spi = _peripheral->spi.spi.handle.Instance; /// it is working too
    // Check if data is transmitted
    while ((spi->SR & SPI_SR_TXE) == 0);
    spi->DR = data;
}

void BurstSPI::fastWriteN(uint16_t data, uint32_t count)
{
    SPI_TypeDef *spi = _peripheral->spi.spi.handle.Instance; /// it is working too

    for (uint32_t i = 0; i < count; ++i) {
        while ((spi->SR & SPI_SR_TXE) == 0);
        spi->DR = data;
    }
}
   
void BurstSPI::clearRX( void )
{
    //const SPI_TypeDef *spi = (SPI_TypeDef *)(_peripheral->spi.spi.spi);
    const SPI_TypeDef *spi = _peripheral->spi.spi.handle.Instance;
    while (spi->SR & SPI_SR_BSY){   
        // Check RX buffer readable
        while ((spi->SR & SPI_SR_RXNE) == 0);
        int dummy = spi->DR;
        (void)dummy; // suppress UNUSED warning 
    }
}


#endif 