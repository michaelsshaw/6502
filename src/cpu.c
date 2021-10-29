#include <cpu.h>
#include <instructions.h>

void
cpu_push(struct em6502 *em, u8 val)
{
    em->mem[em->cpu->SP] = val;
    em->cpu->SP--;
}

u8
cpu_pop(struct em6502 *em)
{
    em->cpu->SP++;
    return em->mem[em->cpu->SP];
}