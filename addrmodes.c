#include <cpu.h>

#include <addrmodes.h>
#include <stdio.h>

#define CPU       cpu
#define PC        CPU->PC
#define AC        CPU->A
#define X         CPU->X
#define Y         CPU->Y
#define MEM(a)    cpu_read(cpu, a)
#define MEMSET(a) cpu_write(cpu, a)
#define MEMS(a)   CPU->mem[a]; // silent mode shhhh
#define CYCLE     CPU->cycles += 1
#define CYCLES    CPU->cycles

#define MODE(mode) CPU->addrmode = mode

ADM_DECL(A) // +0 cycles
{
    MODE(ADDR_ACC);
    return 0x00;
}

ADM_DECL(abs) // +2 cycles
{
    u8  ll = MEM(PC);
    u16 hh = MEM(PC + 1);
    PC += 2;
    MODE(ADDR_ABS);
    return (hh << 0x08) | ll;
}
ADM_DECL(absX) // +2* cycles
{
    u8  ll = MEM(PC);
    u8  hh = MEM(PC + 1);
    u16 r  = ((hh << 0x08) | ll) + X;
    PC += 2;
    MODE(ADDR_ABX);
    if (ll + X > 0xFF)
        CYCLE; // PAGE BOUNDARY CROSS
    return r;
}
ADM_DECL(absY) // +2* cycles
{
    u8  ll = MEM(PC);
    u8  hh = MEM(PC + 1);
    u16 r  = ((hh << 0x08) | ll) + Y;
    PC += 2;
    MODE(ADDR_ABY);
    if (ll + Y > 0xFF)
        CYCLE; // PAGE BOUNDARY CROSS
    return r;
}
ADM_DECL(imm) // +0 cycles
{
    PC += 1;
    MODE(ADDR_IMM);
    return (PC - 1);
}

ADM_DECL(impl) // +0 cycles
{
    MODE(ADDR_IMP);
    return 0x00;
}
ADM_DECL(ind) // +4 cycles
{
    u8 ll = MEM(PC);
    u8 hh = MEM(PC + 1);

    u16 aa = ((hh << 0x08) | ll);
    u16 r;
    if (ll == 0xFF)
    {
        r = ((MEM(aa & 0xFF00) << 8) | MEM(aa));
    }
    else
    {
        r = (MEM((aa + 1)) << 0x08) | (MEM(aa));
    }
    PC += 2;
    MODE(ADDR_IND);
    return r;
}

ADM_DECL(Xind) // +2 cycles
{
    u8 ll = MEM(PC);
    u8 a1 = ll + X + 1;
    u8 a  = ll + X;

    u16 r = (MEM(a1) << 0x08) | MEM(a);
    CYCLE;
    PC += 1;
    MODE(ADDR_XIN);
    return r;
}

ADM_DECL(indY) // +3* cycles
{
    u16 t = 0x0000 | MEM(PC);

    u16 lo = 0x0000 | MEM(t & 0xFF);
    u16 hi = 0x0000 | MEM((t + 1) & 0xFF);

    u16 r = (hi << 8) | lo;
    r += Y;

    if (lo + Y > 0xFF)

        CYCLE; // PAGE BOUNDARY CROSS
    PC += 1;
    MODE(ADDR_INY);
    return r;
}

ADM_DECL(rel) // +1 cycle
{
    u8 bb = MEM(PC);
    PC += 1;
    u16 r = PC;
    MODE(ADDR_REL);
    if ((bb & 0x80) != 0)
    {
        return (r - (((~bb) & 0x00FF) + 1));
    }
    else
    {
        r += bb;
    }
    return r;
}

ADM_DECL(zpg) // +1 cycle
{
    u8 bb = MEM(PC);
    PC += 1;
    MODE(ADDR_ZPG);
    return 0x00FF & bb;
}
ADM_DECL(zpgX) // +2 cycles
{
    u8  bb = MEM(PC);
    u16 r  = 0x00FF & (bb + X);

    PC += 1;
    MODE(ADDR_ZPX);
    CYCLE; // for incrementing by X
    return r;
}
ADM_DECL(zpgY) // +2 cycles
{
    u8  bb = MEM(PC);
    u16 r  = 0x00FF & (bb + Y);

    PC += 1;
    MODE(ADDR_ZPY);
    CYCLE; // for incrementing by Y
    return r;
}
