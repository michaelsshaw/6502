#ifndef ADDRMODES_H_
#define ADDRMODES_H_

#include <em6502.h>

#define ADM_DECL(m) u16 adm_##m(struct em6502 *em)

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