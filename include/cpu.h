#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define ADDR_ACC 0x01
#define ADDR_ABS 0x02
#define ADDR_ABX 0x03
#define ADDR_ABY 0x04
#define ADDR_IMM 0x05
#define ADDR_IMP 0x06
#define ADDR_IND 0x07
#define ADDR_XIN 0x08
#define ADDR_INY 0x09
#define ADDR_REL 0x0A
#define ADDR_ZPG 0x0B
#define ADDR_ZPX 0x0C
#define ADDR_ZPY 0x0D

#define FLAG_C 0x80
#define FLAG_Z 0x40
#define FLAG_5 0x20
#define FLAG_I 0x10
#define FLAG_D 0x08
#define FLAG_B 0x04
#define FLAG_V 0x02
#define FLAG_N 0x01

#define RESET_VECTOR 0xFFFC

#define GETFLAG(em, flag) (((em)->flags & (flag)) == (flag))
#define SETFLAG(em, flag) ((em)->flags |= (flag))
#define CLFLAG(em, flag)  ((em)->flags &= (~(flag)))
#define CLFLAGS(em)       ((em)->flags = 0x00)
#define NEG8(num)         (((num)&0x80) == 1)

#define SETFLAGC(em, flag, cond)                                               \
    ((cond) ? SETFLAG((em), (flag)) : CLFLAG((em), (flag)))

#define MEM_SIZE 65536 // 64MB in bytes

struct cpu
{
    u8  A, X, Y; // registers
    u8  SP;      // stack pointer
    u16 PC;      // program counter
    u8  flags;   // C, Z, (  ), I, D, B, V, N flags;
};

struct em6502
{
    struct cpu *cpu;
    u8         *mem;
};

void
cpu_push(struct em6502 *em, u8 val);

u8
cpu_pop(struct em6502 *em);

#endif // CPU_H_