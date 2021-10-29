#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <em6502.h>

struct em6502 *EM;

int
main()
{
    EM      = malloc(sizeof(struct em6502));
    EM->cpu = malloc(sizeof(struct cpu));
    EM->mem = malloc(MEM_SIZE);

    memset(EM->cpu, 0, sizeof(struct cpu));
    memset(EM->mem, 0, MEM_SIZE);

    return 0;
}