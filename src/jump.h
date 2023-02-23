#ifndef __JUMP_H
#define __JUMP_H

#define NOP_PADDING(fn) \
    __asm__ __volatile__ ( \
        "nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;" \
    );

int InsertJump(void* at, void* to);
#endif