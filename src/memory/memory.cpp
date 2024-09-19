#include "memory.hpp"

void Mem::Init()
{
    for(u32 i = 0; i < MAX_MEM; i++)
    {
        Data[i] = 0;
    }

}

Byte Mem::operator[](u32 Address) const
{
        assert(Address < MAX_MEM);
        return Data[Address];
}

Byte& Mem::operator[](u32 Address)
{
    assert(Address < MAX_MEM);
    return Data[Address];
}

void Mem::WriteWord(u32& Cycles, Word word, u32 Address)
{
    Data[Address] = (word & 0xFF);
    Data[Address+1] = (word >> 8);

    Cycles-=2;
}
