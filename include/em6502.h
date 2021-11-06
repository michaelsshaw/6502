#ifndef EM6502_H_
#define EM6502_H_

void
noprintf(char *s, ...);

struct cpu;

struct em6502
{
    struct cpu *cpu;
};

#endif // EM6502_H_