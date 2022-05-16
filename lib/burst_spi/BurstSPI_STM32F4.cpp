#if defined(TARGET_STM32F4)
#include "BurstSPI.h"
 
/*
#if DEVICE_SPI_ASYNCH
    #define SPI_INST(obj)    ((SPI_TypeDef *)((obj)->spi.spi))
#else
    #define SPI_INST(obj)    ((SPI_TypeDef *)((obj)->spi))
#endif 
*/

void BurstSPI::fastWrite(int data) {
    SPI_TypeDef *spi = _spi.spi.handle.Instance;
    // Check if data is transmitted
    while ((spi->SR & SPI_SR_TXE) == 0);
    spi->DR = data;
}

void BurstSPI::fastWriteN(uint16_t data, uint32_t count)
{
    SPI_TypeDef *spi = _spi.spi.handle.Instance;

    for (uint32_t i = 0; i < count; ++i) {
        while ((spi->SR & SPI_SR_TXE) == 0);
        spi->DR = data;
    }
}
    
void BurstSPI::clearRX( void ) {
    //Check if the RX buffer is busy
    SPI_TypeDef *spi = _spi.spi.handle.Instance;
    //While busy, keep checking
    while (spi->SR & SPI_SR_BSY){   
        // Check RX buffer readable
        while ((spi->SR & SPI_SR_RXNE) == 0);
        int dummy = spi->DR;
    }
}
#endif

