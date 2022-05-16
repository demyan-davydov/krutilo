#include "burst_spi_x.h"
//#include "SPI.h"
#include "spi_api.h"

void BurstSpiX::writeFew(int a, int b)
{
    select();
    spi_master_write(&_peripheral->spi, a);
    spi_master_write(&_peripheral->spi, b);
    deselect();
}

void BurstSpiX::writeFew(int a, int b, int c)
{
    select();
    spi_master_write(&_peripheral->spi, a);
    spi_master_write(&_peripheral->spi, b);
    spi_master_write(&_peripheral->spi, c);
    deselect();
}

void BurstSpiX::writeFew(int a, int b, int c, int d)
{
    select();
    spi_master_write(&_peripheral->spi, a);
    spi_master_write(&_peripheral->spi, b);
    spi_master_write(&_peripheral->spi, c);
    spi_master_write(&_peripheral->spi, d);
    deselect();
}

void BurstSpiX::writeFew(int a, int b, int c, int d, int e)
{
    select();
    spi_master_write(&_peripheral->spi, a);
    spi_master_write(&_peripheral->spi, b);
    spi_master_write(&_peripheral->spi, c);
    spi_master_write(&_peripheral->spi, d);
    spi_master_write(&_peripheral->spi, e);
    deselect();
}

void BurstSpiX::writeFew(int a, int b, int c, int d, int e, int f)
{
    select();
    spi_master_write(&_peripheral->spi, a);
    spi_master_write(&_peripheral->spi, b);
    spi_master_write(&_peripheral->spi, c);
    spi_master_write(&_peripheral->spi, d);
    spi_master_write(&_peripheral->spi, e);
    spi_master_write(&_peripheral->spi, f);
    deselect();
}

