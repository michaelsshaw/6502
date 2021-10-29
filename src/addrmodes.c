#include <addrmodes.h>

#define PC  em->cpu->PC
#define AC  em->cpu->A
#define X   em->cpu->X
#define Y   em->cpu->Y
#define MEM em->mem

ADM_DECL(A)
{
    PC += 1;
    return 0x00;
}

ADM_DECL(abs)
{
    u8  ll = MEM[PC];
    u16 hh = MEM[PC + 1];

    PC += 2;
    return (hh << 0x08) | ll;
}
ADM_DECL(absX)
{
    u8  ll = MEM[PC];
    u16 hh = MEM[PC + 1];

    PC += 2;
    return ((hh << 0x08) | ll) + X;
}
ADM_DECL(absY)
{
    u8  ll = MEM[PC];
    u16 hh = MEM[PC + 1];

    PC += 2;
    return ((hh << 0x08) | ll) + Y;
}
ADM_DECL(imm)
{
    PC += 1;
    return MEM[PC - 1];
}

ADM_DECL(impl)
{
    return 0x00;
}
ADM_DECL(ind)
{
    u8  ll = MEM[PC];
    u8  hh = MEM[PC + 1];
    u16 r  = (MEM[hh] << 0x08) | MEM[ll];

    PC += 2;
    return r;
}

ADM_DECL(Xind)
{
    u8  ll = MEM[PC];
    u16 r  = (0x00FF) & (MEM[ll] + X);

    PC += 1;
    return r;
}

ADM_DECL(indY)
{
    u8  ll = MEM[PC];
    u16 r  = ((0x00FF) & (MEM[ll])) + Y;

    PC += 1;
    return r;
}

ADM_DECL(rel)
{
    u8 bb = MEM[PC];
    PC += 1;
    return PC + bb - 128;
}

ADM_DECL(zpg)
{
    u8 bb = MEM[PC];
    PC += 1;
    return 0x00FF & bb;
}
ADM_DECL(zpgX)
{
    u8 bb = MEM[PC];
    PC += 1;
    return 0x00FF & (bb + X);
}
ADM_DECL(zpgY)
{
    u8 bb = MEM[PC];
    PC += 1;
    return 0x00FF & (bb + Y);
}
