#include <stdio.h>
#include <em6502.h>
#include <instructions.h>
#include <opcodes.h>

#define CPU    em->cpu
#define PC     CPU->PC
#define SP     CPU->SP
#define A      CPU->A
#define X      CPU->X
#define Y      CPU->Y
#define MM     em->mem
#define CYCLE  em->cycles += 1
#define CYCLES em->cycles

#define DBG_FLAG(flag) GETFLAG(CPU, flag) ? #flag[sizeof(#flag) - 2] : '-'

long long cycles = 0;

void
cpu_push(struct em6502 *em, u8 val)
{
    mem_write(em, 0x0100 | SP, val);
    SP -= 1;
}

u8
cpu_pop(struct em6502 *em)
{
    SP += 1;
    return mem_read(em, 0x0100 | SP);
}

u8
mem_read(struct em6502 *em, u16 addr)
{
    CYCLE;
    return MM[addr];
}

void
mem_write(struct em6502 *em, u16 addr, u8 val)
{
    CYCLE;
    MM[addr] = val;
}

void
intr();

void
cpu_clock(struct em6502 *em)
{
    if (cycles == 0)
    {
        u8 opc = mem_read(em, PC);

        noprintf(
          "|SP: %02x|A: %02x|X: %02x|Y: %02x| %c%c%c%c%c%c%c%c || 0x%04X| "
          "0x%02X %s ",
          SP,
          A,
          X,
          Y,
          DBG_FLAG(FLAG_N),
          DBG_FLAG(FLAG_V),
          DBG_FLAG(FLAG_5),
          DBG_FLAG(FLAG_B),
          DBG_FLAG(FLAG_D),
          DBG_FLAG(FLAG_I),
          DBG_FLAG(FLAG_Z),
          DBG_FLAG(FLAG_C),
          PC,
          opc,
          debug_codes[opc]);

        admcall callback_adm = (admcall)codes[opc * 2 + 1];
        inscall callback_ins = (inscall)codes[opc * 2];

        PC += 1;

        if (callback_adm != NULL && callback_ins != NULL)
        {
            u16 addr = callback_adm(em);
            callback_ins(em, addr);
        }

        cycles++;
    }
    CYCLES -= 1;
}

void
prcycles()
{
    printf("\n\n\nTook %lli cycles\n", cycles);
}