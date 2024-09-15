#include "../types/types.hpp"
#ifndef MOS_MEM_DEF
#define MOS_MEM_DEF
#include <assert.h>

struct Mem 
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Init()
    {
        for(u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }

    // read 1 byte
    Byte operator[](u32 Address) const
    {
        assert(Address < MAX_MEM);
        return Data[Address];
    }

    // write 1 byte
    Byte& operator[](u32 Address) 
    {
        assert(Address < MAX_MEM);
        return Data[Address];
    }

    // Write 2 bytes
    void WriteWord( u32& Cycles, Word word, u32 Address)
    {
        Data[Address] = (word & 0xFF);
        Data[Address+1] = (word >> 8);

        Cycles-=2;
    }
};


#endif // !MOS_MEM_DEF


