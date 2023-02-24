#ifndef NOOP__JUMP_H
#define NOOP__JUMP_H

/**
 * @brief Overwrites the first *jump code size* bytes of a function with a jump to the target code.
 *      It uses the current architecture (x86: __i386__ or x64: __x86_64__).
 * @param at The function to overwrite.
 * @param to The target code (0xdeadbeefdeadbeef).
 * @return 0 on success, -1 on failure.
 */
int InsertJump(void* at, void* to);

#endif //NOOP__JUMP_H