#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <time.h>

#include <em6502.h>

struct em6502 *EM;

long long FREQ = 10000000000000L;

int chlast;

int echooff   = 1;

void
noprintf(char *s, ...)
{
    if (!echooff)
    {
        va_list argptr;
        va_start(argptr, s);
        vprintf(s, argptr);
        va_end(argptr);
    }
}

void
intr()
{
    echooff = 0;
}


// MAIN FUNCTION
// generally not used as this is intended as a library
int
main(int argc, char **argv)
{

    if (argc != 2)
    {
        noprintf("Usage: 6502 filename\n");
        return 1;
    }

    EM      = malloc(sizeof(struct em6502));
    EM->cpu = malloc(sizeof(struct cpu));
    EM->mem = malloc(MEM_SIZE);

    memset(EM->cpu, 0, sizeof(struct cpu));
    memset(EM->mem, 0, MEM_SIZE);

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        noprintf("File open failed\n");
        return 1;
    }
    fread(EM->mem, MEM_SIZE, 1, file);
    fclose(file);

    struct timespec nowt;
    long long       last    = 0;
    long long       now     = 0;
    long long       billion = 1000000000L;

    // INIT SETTINGS

    EM->cpu->PC = 0x0400;
    EM->cpu->SP = 0x00;

    // EM->cpu->PC = 0x3364;
    // EM->cpu->SP = 0xFF;
    // EM->cpu->A  = 0x29;
    // EM->cpu->X  = 0x0E;
    // EM->cpu->Y  = 0xFF;  OLD DEBUG STUFF

    // EM->cpu->flags = FLAG_V | FLAG_C;

    // EM->mem[0x0200] = 0x29; // test case

    while (1)
    {
        long long nspercycle = billion / FREQ;

        clock_gettime(CLOCK_REALTIME, &nowt);

        now = ((long long)nowt.tv_sec) * billion + ((long long)nowt.tv_nsec);

        // if (now - last > nspercycle)
        {
            last = now;
            cpu_clock(EM);
        }
    }

    free(EM->mem);
    free(EM->cpu);
    free(EM);

    return 0;
}