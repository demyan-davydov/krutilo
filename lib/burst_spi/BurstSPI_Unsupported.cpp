#if !(defined(TARGET_KL25Z) || defined(TARGET_KL46Z))
#if !(defined(TARGET_LPC1768) || defined(TARGET_LPC1114) || defined(TARGET_LPC11U24) || defined(TARGET_LPC13XX) || defined(TARGET_LPC1549))
#if !(defined(TARGET_NUCLEO_L152RE) || defined(TARGET_STM32F4) || defined(TARGET_STM32F103C8))

#warning BurstSPI target not supported, reverting to regular SPI

#include "BurstSPI.h"

void BurstSPI::fastWrite(int data) {
    write(data);
}

void BurstSPI::fastWriteN(uint16_t data, uint32_t count) {
    select();
    for (uint32_t i = 0; i < count; ++i) {
        spi_master_write(&_peripheral->spi, data);
    }
    deselect();
}

void BurstSPI::clearRX( void ) {

}
#endif          //Freescale
#endif          //NXP
#endif          //NUCLEO