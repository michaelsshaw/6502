#include <stdio.h>

#include <instructions.h>
#include <opcodes.h>

#define CPU    cpu
#define PC     CPU->PC
#define SP     CPU->SP
#define A      CPU->A
#define X      CPU->X
#define Y      CPU->Y
#define MM     CPU->mem
#define CYCLE  CPU->cycles += 1
#define CYCLES CPU->cycles

#define DBG_FLAG(flag) GETFLAG(CPU, flag) ? #flag[sizeof(#flag) - 2] : '-'

#define INRANGE(_n, _a, _b)   (((_n) >= (_a)) && ((_n) < (_a)))
#define IFINRANGE(_m, _c, _d) if (INRANGE((_m), (_c), (_d)))

long long cycles = 0;

u16
nes_cpu_addr(u16 addr)
{
    IFINRANGE(addr, 0x0000, 0x1FFF) //
    {
        return addr & 0x07FF;
    }

    return addr;
}

u8 *
cpu_default_callback(struct cpu *cpu, u16 addr)
{
    return (cpu->mem + addr);
}

void
cpu_set_memcallback(struct cpu *cpu, void *func)
{
    CPU->callback = func;
}

void
cpu_push(struct cpu *cpu, u8 val)
{
    cpu_write(cpu, 0x0100 | SP, val);
    SP -= 1;
}

u8
cpu_pop(struct cpu *cpu)
{
    SP += 1;
    return cpu_read(cpu, 0x0100 | SP);
}

u8
cpu_read(struct cpu *cpu, u16 addr)
{
    cpucallback callback = (cpucallback)cpu->callback;
    CYCLE;
    return *(callback(cpu, addr));
}

void
cpu_write(struct cpu *cpu, u16 addr, u8 val)
{
    cpucallback callback = (cpucallback)cpu->callback;
    u8 *        mem      = (callback(cpu, addr));
    CYCLE;

    *mem = val;
}

void
intr();

void
cpu_clock(struct cpu *cpu)
{
    if (CYCLES == 0)
    {
        u8 opc = cpu_read(cpu, PC);
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
            u16 addr = callback_adm(cpu);
            callback_ins(cpu, addr);
        }
        noprintf("\n");

        cycles++;
    }
    CYCLES -= 1;
}

void
cpu_nmi(struct cpu *cpu)
{

    cpu_push(cpu, (PC >> 0x08));
    cpu_push(cpu, (PC & 0xFF));

    CLFLAG(CPU, FLAG_B);
    SETFLAG(CPU, FLAG_5);
    SETFLAG(CPU, FLAG_I);

    cpu_push(cpu, CPU->flags);

    u8 ll = cpu_read(cpu, 0xFFFA);
    u8 hh = cpu_read(cpu, 0xFFFB);

    PC     = ((hh << 0x08) | ll);
    CYCLES = 7;
}

void
cpu_irq(struct cpu *cpu)
{
    if (!GETFLAG(CPU, FLAG_I))
    {
        cpu_push(cpu, (PC >> 0x08));
        cpu_push(cpu, (PC & 0xFF));

        CLFLAG(CPU, FLAG_B);
        SETFLAG(CPU, FLAG_5);
        SETFLAG(CPU, FLAG_I);

        cpu_push(cpu, CPU->flags);

        u8 ll = cpu_read(cpu, 0xFFFE);
        u8 hh = cpu_read(cpu, 0xFFFF);

        PC     = ((hh << 0x08) | ll);
        CYCLES = 7;
    }
}

void
cpu_rti(struct cpu *cpu)
{
    CPU->flags = cpu_pop(cpu);
    CLFLAG(CPU, FLAG_B);
    CLFLAG(CPU, FLAG_5);

    u8 ll = cpu_pop(cpu);
    u8 hh = cpu_pop(cpu);

    PC = (hh << 0x08) | ll;
}

void
cpu_reset(struct cpu *cpu)
{
    u8 ll = cpu_read(cpu, 0xFFFC);
    u8 hh = cpu_read(cpu, 0xFFFD);

    PC = ((hh << 0x08) | ll);

    CPU->flags = 0x34;
    A          = 0;
    X          = 0;
    Y          = 0;
    SP         = 0xFD;

    CYCLES = 8;
}

void
prcycles()
{
    printf("\n\n\nTook %lli cycles\n", cycles);
}