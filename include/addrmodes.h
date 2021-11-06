#ifndef ADDRMODES_H_
#define ADDRMODES_H_

#include <em6502.h>

#define ADM_DECL(m) u16 adm_##m(struct cpu *cpu)

typedef u16 (*admcall) (struct cpu *cpu);

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

ADM_DECL(A);
ADM_DECL(abs);
ADM_DECL(absX);
ADM_DECL(absY);
ADM_DECL(imm);
ADM_DECL(impl);
ADM_DECL(ind);
ADM_DECL(Xind);
ADM_DECL(indY);
ADM_DECL(rel);
ADM_DECL(zpg);
ADM_DECL(zpgX);
ADM_DECL(zpgY);

#endif // ADDRMODES_H_