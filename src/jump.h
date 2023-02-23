#ifndef __JUMP_H
#define __JUMP_H

/**
 * @brief Overwrites the first *jump code size* bytes of a function with a jump to the target code.
 * @param at The function to overwrite.
 * @param to The target code (0xdeadbeefdeadbeef).
 * @return 0 on success, -1 on failure.
 */
int InsertJump(void* at, void* to);

#endif