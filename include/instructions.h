#ifndef INS_H_
#define INS_H_

#include <cpu.h>
#include <assert.h>

#define INS_DECL(name) inline void inscall_##name(struct em6502 *em, u16 args)

#define CPU em->cpu
#define PC  CPU->PC
#define A   CPU->A
#define X   CPU->X
#define Y   CPU->Y
#define MEM em->mem

INS_DECL(adc)
{
    assert(GETFLAG(CPU, FLAG_D));
    CLFLAG(CPU, FLAG_C);
    CLFLAG(CPU, FLAG_V);
    CLFLAG(CPU, FLAG_N);
    CLFLAG(CPU, FLAG_Z);

    u8  m   = args;
    u16 add = m + GETFLAG(CPU, FLAG_C);

    A += add;

    SETFLAGC(CPU, FLAG_V, A < add);
    SETFLAGC(CPU, FLAG_C, A < add);

    SETFLAGC(CPU, FLAG_Z, A == 0x00);
    SETFLAGC(CPU, FLAG_N, NEG8(A));
}

INS_DECL(and)
{
    CLFLAG(CPU, FLAG_N);
    CLFLAG(CPU, FLAG_Z);

    u8 m = args;

    A &= m;

    SETFLAGC(CPU, FLAG_Z, A == 0x00);
    SETFLAGC(CPU, FLAG_N, NEG8(A));
}

INS_DECL(asl)
{
    // TODO
}

INS_DECL(asl_a)
{
    SETFLAGC(CPU, FLAG_C, NEG8(A));
    A <<= 0x01;
}

INS_DECL(bcc)
{
    if (!GETFLAG(CPU, FLAG_C))
    {
        PC = (args);
    }
}

INS_DECL(bcs)
{
    if (GETFLAG(CPU, FLAG_C))
    {
        PC = (args);
    }
}

INS_DECL(beq)
{
    if (GETFLAG(CPU, FLAG_Z))
    {
        PC = (args);
    }
}

INS_DECL(bit)
{
    u8 m = args;
    m += A;

    SETFLAGC(CPU, FLAG_N, m & 0x80);
    SETFLAGC(CPU, FLAG_Z, m == 0x00);
    SETFLAGC(CPU, FLAG_V, m & 0x40);
}

INS_DECL(bmi)
{
    if (GETFLAG(CPU, FLAG_N))
    {
        PC = (args);
    }
}

INS_DECL(bne)
{
    if (!GETFLAG(CPU, FLAG_Z))
    {
        PC = (args);
    }
}

INS_DECL(bpl)
{
    if (!GETFLAG(CPU, FLAG_Z))
    {
        PC = (args);
    }
}

INS_DECL(brk)
{
    SETFLAG(CPU, FLAG_I);
    u16 p = PC + 2;
    cpu_push(em, p & 0xFF);
    cpu_push(em, (p >> 0x08) & 0xFF);
    cpu_push(em, CPU->flags);
}

INS_DECL(bvc)
{
    if (!GETFLAG(CPU, FLAG_V))
    {
        PC = (args);
    }
}

INS_DECL(bvs)
{
    if (GETFLAG(CPU, FLAG_V))
    {
        PC = (args);
    }
}

INS_DECL(clc)
{
    CLFLAG(CPU, FLAG_C);
}

INS_DECL(cld)
{
    CLFLAG(CPU, FLAG_D);
}

INS_DECL(cli)
{
    CLFLAG(CPU, FLAG_I);
}

INS_DECL(clv)
{
    CLFLAG(CPU, FLAG_V);
}

INS_DECL(cmp)
{
    u8 m = args;
    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == m);
    SETFLAGC(CPU, FLAG_C, A >= m);
}

INS_DECL(cpx)
{
    u8 m = args;
    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == m);
    SETFLAGC(CPU, FLAG_C, X >= m);
}

INS_DECL(cpy)
{
    u8 m = args;
    SETFLAGC(CPU, FLAG_N, NEG8(Y));
    SETFLAGC(CPU, FLAG_Z, Y == m);
    SETFLAGC(CPU, FLAG_C, Y >= m);
}

INS_DECL(dec)
{
    MEM[args] -= 1;

    SETFLAGC(CPU, FLAG_N, NEG8(MEM[args]));
    SETFLAGC(CPU, FLAG_Z, MEM[args] == 0x00);
}

INS_DECL(dex)
{
    X -= 1;

    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == 0x00);
}

INS_DECL(dey)
{
    Y -= 1;

    SETFLAGC(CPU, FLAG_N, NEG8(Y));
    SETFLAGC(CPU, FLAG_Z, Y == 0x00);
}

INS_DECL(eor)
{
    u8 m = args;

    A ^= m;

    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(inc)
{
    MEM[args] += 1;

    SETFLAGC(CPU, FLAG_N, NEG8(MEM[args]));
    SETFLAGC(CPU, FLAG_Z, MEM[args] == 0x00);
}

INS_DECL(inx)
{
    X += 1;

    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == 0x00);
}

INS_DECL(iny)
{
    Y += 1;

    SETFLAGC(CPU, FLAG_N, NEG8(Y));
    SETFLAGC(CPU, FLAG_Z, Y == 0x00);
}

INS_DECL(jmp)
{
    PC = (args);
}

INS_DECL(jsr)
{
    cpu_push(em, ((args >> 0x08) & 0xFF));

    PC = args;
}

INS_DECL(lda)
{
    A = (args & 0xFF);
    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(ldx)
{
    X = (args & 0xFF);
    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == 0x00);
}

INS_DECL(ldy)
{
    Y = (args & 0xFF);
    SETFLAGC(CPU, FLAG_N, NEG8(Y));
    SETFLAGC(CPU, FLAG_Z, Y == 0x00);
}

INS_DECL(lsr)
{
    SETFLAGC(CPU, FLAG_C, (MEM[args & 0xFF] & 0x01));
    MEM[args & 0xFF] >>= 0x01;
    SETFLAGC(CPU, FLAG_Z, MEM[args & 0xFF] == 0);
}

INS_DECL(lsr_a)
{
    SETFLAGC(CPU, FLAG_C, (A & 0x01));
    A >>= 0x01;
    SETFLAGC(CPU, FLAG_Z, A == 0);
}

INS_DECL(nop) {}

INS_DECL(ora)
{
    A |= (args & 0xFF);
    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(pha)
{
    cpu_push(em, A);
}

INS_DECL(php)
{
    cpu_push(em, (CPU->flags | FLAG_B | FLAG_5));
}

INS_DECL(pla)
{
    A = cpu_pop(em);
}

INS_DECL(plp)
{
    CPU->flags = cpu_pop(em);
}

INS_DECL(rol)
{
    SETFLAGC(CPU, FLAG_C, NEG8(MEM[args]));

    MEM[args] <<= 0x01;

    SETFLAGC(CPU, FLAG_N, NEG8(MEM[args]));
    SETFLAGC(CPU, FLAG_Z, MEM[args] == 0x00);
}

INS_DECL(rol_a)
{
    SETFLAGC(CPU, FLAG_C, NEG8(A));

    A <<= 0x01;

    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(ror)
{
    SETFLAGC(CPU, FLAG_C, (MEM[args] & 0x01));

    MEM[args] >>= 0x01;

    SETFLAGC(CPU, FLAG_N, NEG8(MEM[args]));
    SETFLAGC(CPU, FLAG_Z, MEM[args] == 0x00);
}

INS_DECL(ror_a)
{
    SETFLAGC(CPU, FLAG_C, (A & 0x01));

    A >>= 0x01;

    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(rti)
{
    CPU->flags = (cpu_pop(em) & 0xDB);

    u8  l = cpu_pop(em);
    u16 f = ((u16)(cpu_pop(em)) << 0x08) | l;

    PC = f;
}

INS_DECL(rts)
{
    u8  l = cpu_pop(em);
    u16 f = ((u16)(cpu_pop(em)) << 0x08) | l;

    PC = f + 1;
}

INS_DECL(sbc)
{
    assert(GETFLAG(CPU, FLAG_D) == 0);

    u8  m = args;
    u16 t = A - m - (GETFLAG(CPU, FLAG_C) ? 0 : 1);

    CLFLAG(CPU, FLAG_C);
    CLFLAG(CPU, FLAG_V);
    CLFLAG(CPU, FLAG_N);
    CLFLAG(CPU, FLAG_Z);

    SETFLAGC(CPU, FLAG_V, t > 0xFF);
    SETFLAGC(CPU, FLAG_C, t >= 0);
    SETFLAGC(CPU, FLAG_Z, t == 0x00);
    SETFLAGC(CPU, FLAG_N, NEG8(A));

    A = t & 0xFF;
}

INS_DECL(sec)
{
    SETFLAG(CPU, FLAG_C);
}

INS_DECL(sed)
{
    SETFLAG(CPU, FLAG_D);
}

INS_DECL(sei)
{
    SETFLAG(CPU, FLAG_I);
}

INS_DECL(sta)
{
    MEM[args] = A;
}

INS_DECL(stx)
{
    MEM[args] = X;
}

INS_DECL(sty)
{
    MEM[args] = Y;
}

INS_DECL(tax)
{
    X = A;
    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == 0x00);
}

INS_DECL(tay)
{
    Y = A;
    SETFLAGC(CPU, FLAG_N, NEG8(Y));
    SETFLAGC(CPU, FLAG_Z, Y == 0x00);
}

INS_DECL(tsx)
{
    X = CPU->SP;
    SETFLAGC(CPU, FLAG_N, NEG8(X));
    SETFLAGC(CPU, FLAG_Z, X == 0x00);
}

INS_DECL(txa)
{
    A = X;
    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

INS_DECL(txs)
{
    CPU->SP = X;
}

INS_DECL(tya)
{
    A = Y;
    SETFLAGC(CPU, FLAG_N, NEG8(A));
    SETFLAGC(CPU, FLAG_Z, A == 0x00);
}

#endif // INS_H_