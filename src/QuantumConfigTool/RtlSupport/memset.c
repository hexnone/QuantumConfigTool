#include "RtlSupportTypes.h"

void* __cdecl memset(void* ptr, int value, size_t num)
{
    unsigned char* p = (unsigned char*)ptr;
    while (num > 0)
    {
        *p = value;
        p++;
        num--;
    }
    return ptr;
}