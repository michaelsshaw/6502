#ifndef EM6502_H_
#define EM6502_H_

void
noprintf(char *s, ...);

struct cpu;

struct em6502
{
    struct cpu *cpu;
    u8         *mem;
    u8          cycles;
    u8          addrmode;
};

#endif // EM6502_H_