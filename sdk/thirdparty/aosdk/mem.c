#include "mem.h"

uint8 memory_read(uint16 addr)
{
    return qsf_memory_read(addr);
}

uint8 memory_readop(uint16 addr)
{
    return memory_read(addr);
}

uint8 memory_readport(uint16 addr)
{
    return qsf_memory_readport(addr);
}

void memory_write(uint16 addr, uint8 byte)
{
    qsf_memory_write(addr, byte);
}

void memory_writeport(uint16 addr, uint8 byte)
{
    qsf_memory_writeport(addr, byte);
}
