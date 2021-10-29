#ifndef INS_H_
#define INS_H_

#include <cpu.h>
#include <assert.h>

#define INS_DECL(name) inline void inscall_##name(struct em6502 *em, u16 args)

INS_DECL(adc)
{
    assert(GETFLAG(em->cpu, FLAG_D));
    CLFLAG(em->cpu, FLAG_C);
    CLFLAG(em->cpu, FLAG_V);
    CLFLAG(em->cpu, FLAG_N);
    CLFLAG(em->cpu, FLAG_Z);

    u8  m   = args;
    u16 add = m + GETFLAG(em->cpu, FLAG_C);

    em->cpu->A += add;

    SETFLAGC(em->cpu, FLAG_V, em->cpu->A < add);
    SETFLAGC(em->cpu, FLAG_C, em->cpu->A < add);

    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == 0x00);
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));
}

INS_DECL(and)
{
    CLFLAG(em->cpu, FLAG_N);
    CLFLAG(em->cpu, FLAG_Z);

    u8 m = args;

    em->cpu->A &= m;

    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == 0x00);
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));
}

INS_DECL(asl)
{
    // TODO
}

INS_DECL(asl_a)
{
    SETFLAGC(em->cpu, FLAG_C, NEG8(em->cpu->A));
    em->cpu->A <<= 0x01;
}

INS_DECL(bcc)
{
    if (!GETFLAG(em->cpu, FLAG_C))
    {
        em->cpu->PC = (args);
    }
}

INS_DECL(bcs)
{
    if (GETFLAG(em->cpu, FLAG_C))
    {
        em->cpu->PC = (args);
    }
}

INS_DECL(beq)
{
    if (GETFLAG(em->cpu, FLAG_Z))
    {
        em->cpu->PC = (args);
    }
}

INS_DECL(bit)
{
    u8 m = args;
    m += em->cpu->A;

    SETFLAGC(em->cpu, FLAG_N, m & 0x80);
    SETFLAGC(em->cpu, FLAG_Z, m == 0x00);
    SETFLAGC(em->cpu, FLAG_V, m & 0x40);
}

INS_DECL(bmi)
{
    if (GETFLAG(em->cpu, FLAG_N))
    {
        em->cpu->PC = (args);
    }
}

INS_DECL(bne)
{
    if (!GETFLAG(em->cpu, FLAG_Z))
    {
        em->cpu->PC = (args);
    }
}

INS_DECL(bpl)
{
    if (!GETFLAG(em->cpu, FLAG_Z))
    {
        em->cpu->PC = (args);
    }
}

INS_DECL(brk)
{
    SETFLAG(em->cpu, FLAG_I);
    u16 p = em->cpu->PC + 2;
    cpu_push(em, p & 0xFF);
    cpu_push(em, (p >> 0x08) & 0xFF);
    cpu_push(em, em->cpu->flags);
}

INS_DECL(bvc)
{
    if (!GETFLAG(em->cpu, FLAG_V))
    {
        em->cpu->PC = (args);
    }
}

INS_DECL(bvs)
{
    if (GETFLAG(em->cpu, FLAG_V))
    {
        em->cpu->PC = (args);
    }
}

INS_DECL(clc)
{
    CLFLAG(em->cpu, FLAG_C);
}

INS_DECL(cld)
{
    CLFLAG(em->cpu, FLAG_D);
}

INS_DECL(cli)
{
    CLFLAG(em->cpu, FLAG_I);
}

INS_DECL(clv)
{
    CLFLAG(em->cpu, FLAG_V);
}

INS_DECL(cmp)
{
    u8 m = args;
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == m);
    SETFLAGC(em->cpu, FLAG_C, em->cpu->A >= m);
}

INS_DECL(cpx)
{
    u8 m = args;
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->X));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->X == m);
    SETFLAGC(em->cpu, FLAG_C, em->cpu->X >= m);
}

INS_DECL(cpy)
{
    u8 m = args;
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->Y));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->Y == m);
    SETFLAGC(em->cpu, FLAG_C, em->cpu->Y >= m);
}

INS_DECL(dec)
{
    em->mem[args] -= 1;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->mem[args]));
    SETFLAGC(em->cpu, FLAG_Z, em->mem[args] == 0x00);
}

INS_DECL(dex)
{
    em->cpu->X -= 1;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->X));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->X == 0x00);
}

INS_DECL(dey)
{
    em->cpu->Y -= 1;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->Y));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->Y == 0x00);
}

INS_DECL(eor)
{
    u8 m = args;

    em->cpu->A ^= m;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == 0x00);
}

INS_DECL(inc)
{
    em->mem[args] += 1;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->mem[args]));
    SETFLAGC(em->cpu, FLAG_Z, em->mem[args] == 0x00);
}

INS_DECL(inx)
{
    em->cpu->X += 1;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->X));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->X == 0x00);
}

INS_DECL(iny)
{
    em->cpu->Y += 1;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->Y));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->Y == 0x00);
}

INS_DECL(jmp)
{
    em->cpu->PC = (args);
}

INS_DECL(jsr)
{
    cpu_push(em, (args & 0xFF));
    cpu_push(em, ((args >> 0x08) & 0xFF));

    em->cpu->PC = args;
}

INS_DECL(lda)
{
    em->cpu->A = (args & 0xFF);
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == 0x00);
}

INS_DECL(ldx)
{
    em->cpu->X = (args & 0xFF);
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->X));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->X == 0x00);
}

INS_DECL(ldy)
{
    em->cpu->Y = (args & 0xFF);
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->Y));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->Y == 0x00);
}

INS_DECL(lsr)
{
    SETFLAGC(em->cpu, FLAG_C, (em->mem[args & 0xFF] & 0x01));
    em->mem[args & 0xFF] >>= 0x01;
    SETFLAGC(em->cpu, FLAG_Z, em->mem[args & 0xFF] == 0);
}

INS_DECL(lsr_a)
{
    SETFLAGC(em->cpu, FLAG_C, (em->cpu->A & 0x01));
    em->cpu->A >>= 0x01;
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == 0);
}

INS_DECL(nop) {}

INS_DECL(ora)
{
    em->cpu->A |= (args & 0xFF);
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == 0x00);
}

INS_DECL(pha)
{
    cpu_push(em, em->cpu->A);
}

INS_DECL(php)
{
    cpu_push(em, (em->cpu->flags | FLAG_B | FLAG_5));
}

INS_DECL(pla)
{
    em->cpu->A = cpu_pop(em);
}

INS_DECL(plp)
{
    em->cpu->flags = cpu_pop(em);
}

INS_DECL(rol)
{
    SETFLAGC(em->cpu, FLAG_C, NEG8(em->mem[args]));

    em->mem[args] <<= 0x01;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->mem[args]));
    SETFLAGC(em->cpu, FLAG_Z, em->mem[args] == 0x00);
}

INS_DECL(rol_a)
{
    SETFLAGC(em->cpu, FLAG_C, NEG8(em->cpu->A));

    em->cpu->A <<= 0x01;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == 0x00);
}

INS_DECL(ror)
{
    SETFLAGC(em->cpu, FLAG_C, (em->mem[args] & 0x01));

    em->mem[args] >>= 0x01;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->mem[args]));
    SETFLAGC(em->cpu, FLAG_Z, em->mem[args] == 0x00);
}

INS_DECL(ror_a)
{
    SETFLAGC(em->cpu, FLAG_C, (em->cpu->A & 0x01));

    em->cpu->A >>= 0x01;

    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == 0x00);
}

INS_DECL(rti)
{
    em->cpu->flags = (cpu_pop(em) & 0xDB);

    u8  l = cpu_pop(em);
    u16 f = ((u16)(cpu_pop(em)) << 0x08) | l;

    em->cpu->PC = f;
}

INS_DECL(rts)
{
    u8  l = cpu_pop(em);
    u16 f = ((u16)(cpu_pop(em)) << 0x08) | l;

    em->cpu->PC = f + 1;
}

INS_DECL(sbc)
{
    assert(GETFLAG(em->cpu, FLAG_D) == 0);

    u8  m = args;
    u16 t = em->cpu->A - m - (GETFLAG(em->cpu, FLAG_C) ? 0 : 1);

    CLFLAG(em->cpu, FLAG_C);
    CLFLAG(em->cpu, FLAG_V);
    CLFLAG(em->cpu, FLAG_N);
    CLFLAG(em->cpu, FLAG_Z);

    SETFLAGC(em->cpu, FLAG_V, t > 0xFF);
    SETFLAGC(em->cpu, FLAG_C, t >= 0);
    SETFLAGC(em->cpu, FLAG_Z, t == 0x00);
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));

    em->cpu->A = t & 0xFF;
}

INS_DECL(sec)
{
    SETFLAG(em->cpu, FLAG_C);
}

INS_DECL(sed)
{
    SETFLAG(em->cpu, FLAG_D);
}

INS_DECL(sei)
{
    SETFLAG(em->cpu, FLAG_I);
}

INS_DECL(sta)
{
    em->mem[args] = em->cpu->A;
}

INS_DECL(stx)
{
    em->mem[args] = em->cpu->X;
}

INS_DECL(sty)
{
    em->mem[args] = em->cpu->Y;
}

INS_DECL(tax)
{
    em->cpu->X = em->cpu->A;
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->X));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->X == 0x00);
}

INS_DECL(tay)
{
    em->cpu->Y = em->cpu->A;
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->Y));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->Y == 0x00);
}

INS_DECL(tsx)
{
    em->cpu->X = em->cpu->SP;
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->X));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->X == 0x00);
}

INS_DECL(txa)
{
    em->cpu->A = em->cpu->X;
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == 0x00);
}

INS_DECL(txs)
{
    em->cpu->SP = em->cpu->X;
}

INS_DECL(tya)
{
    em->cpu->A = em->cpu->Y;
    SETFLAGC(em->cpu, FLAG_N, NEG8(em->cpu->A));
    SETFLAGC(em->cpu, FLAG_Z, em->cpu->A == 0x00);
}

#endif // INS_H_