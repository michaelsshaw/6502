#include <instructions.h>
#include <stdlib.h>
#include <stdio.h>

#define CPU          cpu
#define PC           CPU->PC
#define A            CPU->A
#define X            CPU->X
#define Y            CPU->Y
#define MEM(a)       cpu_read(CPU, a)
#define MEMSET(a, v) cpu_write(CPU, a, v)
#define MEMS(a)      CPU->mem[a]; // silent mode shhhh
#define CYCLE        CPU->cycles += 1
#define CYCLES       CPU->cycles
#define CSET(a)      CPU->cycles = a;

#define DSIGN(c) (d && invert) ? -(c) : +(c);

#define ADM CPU->addrmode

int invert = 0;

INS_DECL(adc) //
{

    u8 m = MEM(addr);

    if (invert)
    {
        invert = 0;
        m      = ~m;
    }

    u8 c = GETFLAG(CPU, FLAG_C);

    u16 sum = A + m + c;
    SETFLAGC(CPU, FLAG_C, sum > 0xFF);
    SETFLAGC(CPU, FLAG_V, (~(A ^ m) & (A ^ sum)) & 0x80);

    A = sum;

    SETFLAGC(CPU, FLAG_Z, A == 0x00);
    SETFLAGC(CPU, FLAG_N, NEG8(A));
}

INS_DECL(and) //
{

    u8 m = MEM(addr);

    A &= m;

    SETFLAGC(CPU, FLAG_Z, A == 0x00);
    SETFLAGC(CPU, FLAG_N, NEG8(A));
}

INS_DECL(asl) //
{
    u8 m = MEM(addr);
    SETFLAGC(CPU, FLAG_C, NEG8(m));

    m <<= 0x01;
    CYCLE;

    MEMSET(addr, m);

    if (ADM == ADDR_ABX)
        CSET(7);

    SETFLAGC(CPU, FLAG_Z, m == 0);
    SETFLAGC(CPU, FLAG_N, NEG8(m));
}

INS_DECL(asl_a) //
{
    SETFLAGC(CPU, FLAG_C, NEG8(A));

    A <<= 0x01;
    CYCLE;

    SETFLAGC(CPU, FLAG_Z, A == 0);
    SETFLAGC(CPU, FLAG_N, NEG8(A));
}

INS_DECL(bcc) //
{
    if (!GETFLAG(CPU, FLAG_C))
    {
        CYCLE;
        if (((addr)&0xFF00) != (PC & 0xFF00))
            CYCLE; // PAGE CROSS
        PC = addr;
    }
}

INS_DECL(bcs) //
{
    if (GETFLAG(CPU, FLAG_C))
    {
        CYCLE;
        if (((addr)&0xFF00) != (PC & 0xFF00))
            CYCLE; // PAGE CROSS
        PC = (addr);
    }
}

INS_DECL(beq) //
{
    if (GETFLAG(CPU, FLAG_Z))
    {
        CYCLE;
        if (((addr)&0xFF00) != (PC & 0xFF00))
            CYCLE; // PAGE CROSS

        PC = (addr);
    }
}

INS_DECL(bit) //
{
    u8 m = MEM(addr);

    SETFLAGC(CPU, FLAG_N, m & 0x80);
    SETFLAGC(CPU, FLAG_V, m & 0x40);

    m &= A;

    SETFLAGC(CPU, FLAG_Z, m == 0x00);
}

INS_DECL(bmi) //
{
    if (GETFLAG(CPU, FLAG_N))
    {
        CYCLE;
        if (((addr)&0xFF00) != (PC & 0xFF00))
            CYCLE; // PAGE CROSS

        PC = (addr);
    }
}

void
prcycles();

INS_DECL(bne) //
{
    if (!GETFLAG(CPU, FLAG_Z))
    {
        CYCLE;
        if (((addr)&0xFF00) != (PC & 0xFF00))
            CYCLE; // PAGE CROSS

        if ((PC - 2) == addr)
        {
            prcycles();
            exit(0);
        }
        PC = (addr);
    }
}

INS_DECL(bpl) //
{
    if (!GETFLAG(CPU, FLAG_N))
    {
        CYCLE;
        if (((addr)&0xFF00) != (PC & 0xFF00))
            CYCLE; // PAGE CROSS

        PC = (addr);
    }
}

INS_DECL(brk) //
{
    u16 p = PC + 1;
    cpu_push(CPU, (p >> 0x08) & 0xFF);
    cpu_push(CPU, p & 0xFF);
    cpu_push(CPU, CPU->flags | FLAG_B | FLAG_5);

    SETFLAG(CPU, FLAG_I);
    u8 ll = MEM(0xFFFE);
    u8 hh = MEM(0xFFFF);

    CYCLE;

    PC = (hh << 0x08) | ll;
}

INS_DECL(bvc) //
{
    if (!GETFLAG(CPU, FLAG_V))
    {
        CYCLE;
        if (((addr)&0xFF00) != (PC & 0xFF00))
            CYCLE; // PAGE CROSS

        PC = (addr);
    }
}

INS_DECL(bvs) //
{
    if (GETFLAG(CPU, FLAG_V))
    {
        CYCLE;
        if (((addr)&0xFF00) != (PC & 0xFF00))
            CYCLE; // PAGE CROSS

        PC = (addr);
    }
}

INS_DECL(clc) //
{
    CLFLAG(CPU, FLAG_C);
    CYCLE;
}

INS_DECL(cld) //
{
    CLFLAG(CPU, FLAG_D);
    CYCLE;
}

INS_DECL(cli) //
{
    CLFLAG(CPU, FLAG_I);
    CYCLE;
}

INS_DECL(clv) //
{
    CLFLAG(CPU, FLAG_V);
    CYCLE;
}
INS_DECL(cmp) //
{
    u8 m = MEM(addr);
    u8 a = A - m;
    // noprintf(" --- (EXP: %02x, VAL: %02x)", m, A);
    SETFLAGC(CPU, FLAG_N, NEG8(a));
    SETFLAGC(CPU, FLAG_Z, A == m);
    SETFLAGC(CPU, FLAG_C, A >= m);
}

INS_DECL(cpx) //
{
    u8 m = MEM(addr);
    u8 a = X - m;
    SETFLAGC(CPU, FLAG_N, NEG8(a));
    SETFLAGC(CPU, FLAG_Z, X == m);
    SETFLAGC(CPU, FLAG_C, X >= m);
}

INS_DECL(cpy) //
{
    u8 m = MEM(addr);
    u8 a = Y - m;
    SETFLAGC(CPU, FLAG_N, NEG8(a));
    SETFLAGC(CPU, FLAG_Z, Y == m);
    SETFLAGC(CPU, FLAG_C, Y >= m);
}

INS_DECL(dec) //
{
    u8 m = MEM(addr);

    m -= 1;
    CYCLE;

    MEMSET(addr, m);

    if (ADM == ADDR_ABX)
        CSET(7);

    SETFLAGC(CPU, FLAG_N, NEG8(m));
    SETFLAGC(CPU, FLAG_Z, m == 0x00);
}

INS_DECL(dex) //
{
    X -= 1;
    CYCLE;

    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == 0x00);
}

INS_DECL(dey) //
{
    Y -= 1;
    CYCLE;

    SETFLAGC(CPU, FLAG_N, NEG8(Y));
    SETFLAGC(CPU, FLAG_Z, Y == 0x00);
}

INS_DECL(eor) //
{
    u8 m = MEM(addr);

    A ^= m;

    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(inc) //
{
    u8 m = MEM(addr);

    m += 1;
    CYCLE;

    MEMSET(addr, m);

    if (ADM == ADDR_ABX)
        CSET(7);

    SETFLAGC(CPU, FLAG_N, NEG8(m));
    SETFLAGC(CPU, FLAG_Z, m == 0x00);
}

INS_DECL(inx) //
{
    X += 1;
    CYCLE;

    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == 0x00);
}

INS_DECL(iny) //
{
    Y += 1;
    CYCLE;

    SETFLAGC(CPU, FLAG_N, NEG8(Y));
    SETFLAGC(CPU, FLAG_Z, Y == 0x00);
}

INS_DECL(jmp) //
{
    PC = (addr);
}

INS_DECL(jsr) //
{
    cpu_push(CPU, (((PC - 1) >> 0x08) & 0xFF));
    cpu_push(CPU, ((PC - 1) & 0xFF));
    PC = addr;

    CYCLE;
}

INS_DECL(lda) //
{
    A = MEM(addr);

    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(ldx) //
{
    X = MEM(addr);

    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == 0x00);
}

INS_DECL(ldy) //
{
    Y = MEM(addr);

    SETFLAGC(CPU, FLAG_N, NEG8(Y));
    SETFLAGC(CPU, FLAG_Z, Y == 0x00);
}

INS_DECL(lsr) //
{
    u8 m = MEM(addr);

    SETFLAGC(CPU, FLAG_C, (m & 0x01));

    m >>= 0x01;
    CYCLE;

    SETFLAGC(CPU, FLAG_Z, m == 0);
    CLFLAG(CPU, FLAG_N);

    MEMSET(addr, m);

    if (ADM == ADDR_ABX)
        CSET(7);
}

INS_DECL(lsr_a) //
{
    SETFLAGC(CPU, FLAG_C, (A & 0x01));

    A >>= 0x01;
    CYCLE;

    SETFLAGC(CPU, FLAG_Z, A == 0);
    CLFLAG(CPU, FLAG_N);
}

INS_DECL(nop) //
{
    CYCLE;
}

INS_DECL(ora) //
{
    u8 m = MEM(addr);

    A |= m;
    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(pha) //
{
    cpu_push(CPU, A);
    CYCLE;
}

INS_DECL(php) //
{
    cpu_push(CPU, (CPU->flags | FLAG_B | FLAG_5));
    CYCLE;
}

INS_DECL(pla) //
{
    A = cpu_pop(CPU);

    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);

    CYCLE;
    CYCLE;
}

INS_DECL(plp) //
{
    CPU->flags = (cpu_pop(CPU) & (~(FLAG_5 | FLAG_B)));

    CYCLE;
    CYCLE;
}

INS_DECL(rol) //
{
    u8 m = MEM(addr);
    // I'm gonna keep this line commented because this
    // tiny issue, literally a single letter, caused a very confusing bug where
    // many tiles in Super Mario Bros would end up green, not their correct
    // color
    //
    // Solution found at: https://forums.nesdev.org/viewtopic.php?p=163890#p163890
    //    int neg = NEG8(A);
    int neg = NEG8(m);

    m <<= 0x01;
    m |= GETFLAG(CPU, FLAG_C);
    CYCLE;

    MEMSET(addr, m);

    if (ADM == ADDR_ABX)
        CSET(7);

    SETFLAGC(CPU, FLAG_C, neg);
    SETFLAGC(CPU, FLAG_N, NEG8(m));
    SETFLAGC(CPU, FLAG_Z, m == 0x00);
}

INS_DECL(rol_a) //
{

    int neg = NEG8(A);
    A <<= 0x01;
    A |= GETFLAG(CPU, FLAG_C);
    CYCLE;

    SETFLAGC(CPU, FLAG_C, neg);
    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(ror) //
{
    u8 m = MEM(addr);

    int c = m & 0x01;

    m >>= 0x01;
    m |= (GETFLAG(CPU, FLAG_C) << 0x07);
    CYCLE;

    MEMSET(addr, m);

    if (ADM == ADDR_ABX)
        CSET(7);

    SETFLAGC(CPU, FLAG_C, c);
    SETFLAGC(CPU, FLAG_N, NEG8(m));
    SETFLAGC(CPU, FLAG_Z, m == 0x00);
}

INS_DECL(ror_a) //
{
    int c = A & 0x01;

    A >>= 0x01;
    A |= (GETFLAG(CPU, FLAG_C) << 0x07);

    SETFLAGC(CPU, FLAG_C, c);
    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(rti) //
{
    CPU->flags = (cpu_pop(CPU) & (~(FLAG_5 | FLAG_B)));

    u8  l = cpu_pop(CPU);
    u16 f = ((u16)(cpu_pop(CPU)) << 0x08) | l;

    PC = f;

    CYCLE;
    CYCLE;
}

INS_DECL(rts) //
{
    u8  l = cpu_pop(CPU);
    u16 f = ((u16)(cpu_pop(CPU)) << 0x08) | l;

    PC = f + 1;

    CYCLE;
    CYCLE;
    CYCLE;
}

INS_DECL(sbc) //
{
    invert = 1;
    inscall_adc(CPU, addr);
}

INS_DECL(sec) //
{
    SETFLAG(CPU, FLAG_C);
    CYCLE;
}

INS_DECL(sed) //
{
    SETFLAG(CPU, FLAG_D);
    CYCLE;
}

INS_DECL(sei) //
{
    SETFLAG(CPU, FLAG_I);
    CYCLE;
}

INS_DECL(sta) //
{
    MEMSET(addr, A);

    if (ADM == ADDR_ABX || ADM == ADDR_ABY)
        CSET(5);

    if (ADM == ADDR_XIN || ADM == ADDR_INY)
        CSET(6);
}

INS_DECL(stx) //
{
    MEMSET(addr, X);
}

INS_DECL(sty) //
{
    MEMSET(addr, Y);
}

INS_DECL(tax)
{
    X = A;
    CYCLE;

    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == 0x00);
}

INS_DECL(tay)
{
    Y = A;
    CYCLE;

    SETFLAGC(CPU, FLAG_N, NEG8(Y));
    SETFLAGC(CPU, FLAG_Z, Y == 0x00);
}

INS_DECL(tsx)
{
    X = CPU->SP;
    CYCLE;

    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == 0x00);
}

INS_DECL(txa)
{
    A = X;
    CYCLE;

    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(txs)
{
    CPU->SP = X;
    CYCLE;
}

INS_DECL(tya)
{
    A = Y;
    CYCLE;

    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}
