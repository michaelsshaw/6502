/*
 * MIT License
 *
 * Copyright 2021 Michael Shaw
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef INS_H_
#define INS_H_

#include <cpu.h>
#include <addrmodes.h>
#include <assert.h>

#define INS_DECL(name) void inscall_##name(struct cpu *cpu, u16 addr)

typedef void (*inscall)(struct cpu *cpu, u16 addr);

INS_DECL(adc);
INS_DECL(and);
INS_DECL(asl);
INS_DECL(asl_a);
INS_DECL(bcc);
INS_DECL(bcs);
INS_DECL(beq);
INS_DECL(bit);
INS_DECL(bmi);
INS_DECL(bne);
INS_DECL(bpl);
INS_DECL(brk);
INS_DECL(bvc);
INS_DECL(bvs);
INS_DECL(clc);
INS_DECL(cld);
INS_DECL(cli);
INS_DECL(clv);
INS_DECL(cmp);
INS_DECL(cpx);
INS_DECL(cpy);
INS_DECL(dec);
INS_DECL(dex);
INS_DECL(dey);
INS_DECL(eor);
INS_DECL(inc);
INS_DECL(inx);
INS_DECL(iny);
INS_DECL(jmp);
INS_DECL(jsr);
INS_DECL(lda);
INS_DECL(ldx);
INS_DECL(ldy);
INS_DECL(lsr);
INS_DECL(lsr_a);
INS_DECL(nop);
INS_DECL(ora);
INS_DECL(pha);
INS_DECL(php);
INS_DECL(pla);
INS_DECL(plp);
INS_DECL(rol);
INS_DECL(rol_a);
INS_DECL(ror);
INS_DECL(ror_a);
INS_DECL(rti);
INS_DECL(rts);
INS_DECL(sbc);
INS_DECL(sec);
INS_DECL(sed);
INS_DECL(sei);
INS_DECL(sta);
INS_DECL(stx);
INS_DECL(sty);
INS_DECL(tax);
INS_DECL(tay);
INS_DECL(tsx);
INS_DECL(txa);
INS_DECL(txs);
INS_DECL(tya);

#endif // INS_H_
