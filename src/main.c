#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#include <time.h>

#include <em6502.h>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

struct em6502 *EM;

long long FREQ = 10000000000000L;

int chlast;

int interrupt = 0;
int echooff   = 1;

void
noprintf(char *s, ...)
{
    if(!echooff)
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

int
kbhit()
{
    struct termios oldt, newt;
    int            ch;
    int            oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        return ch;
    }

    return 0;
}

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

    // EM->cpu->PC = 0x0400;
    // EM->cpu->SP = 0x00;

    EM->cpu->PC = 0x3364;
    EM->cpu->SP = 0xFF;
    EM->cpu->A  = 0x29;
    EM->cpu->X  = 0x0E;
    EM->cpu->Y  = 0xFF;

    EM->cpu->flags = FLAG_V | FLAG_C;

    EM->mem[0x0200] = 0x29; // test case

    while (1)
    {
        long long nspercycle = billion / FREQ;

        int k = kbhit();
        if (k == ('d' & 037))
        {
            interrupt = !(interrupt);
            noprintf("SIGINT\n");
        }

        if (k == 'w')
        {
            FREQ *= 2;
            if (FREQ < 1)
            {
                FREQ = 1;
            }

            noprintf("FREQ=%lliHz\n", FREQ);
        }

        if (k == 's')
        {
            FREQ /= 2;
            if (FREQ < 1)
            {
                FREQ = 1;
            }
            noprintf("FREQ=%lliHz\n", FREQ);
        }

        if (k == 'q')
        {
            interrupt = 1;
            int dd;
            scanf("%x", &dd);
            noprintf("MEM=%04X", EM->mem[dd]);
        }

        if (k == 'r')
        {
            if (echooff)
            {
                echooff = 0;
            }
            else
            {
                echooff = 1;
            }
        }

        if (interrupt)
        {
            continue;
        }

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