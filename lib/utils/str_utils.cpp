#include "str_utils.h"
#include <algorithm>

// reverse string
static void reverse(char* buff, char* buff_end)
{
    --buff_end;

    while(buff < buff_end)
    {
        std::swap(*buff++, *buff_end--);
    }
}

char* print_num(char* buf, unsigned int num, unsigned int leading_zeros = 0)
{
    char* begin = buf;
    do
    {
        *buf = '0' + (num % 10);
        num /= 10;
    } while (num);
    
    const char* zto = begin + leading_zeros;
    while(buf < zto) {
        *buf++ = '0';
    }

    *buf = 0;
    reverse(begin, buf);
    return buf;
}