#ifndef LIBASM_H
#define LIBASM_H

extern uint64_t sys_call(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t r10, uint64_t r8);
extern _hlt();
extern _cli();
extern _sti();
extern random();

#endif // LIBASM_H