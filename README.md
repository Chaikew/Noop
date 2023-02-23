## Acrobatic Overwriting
This repo contains some C x64 trash code that can be used to "replace C functions". 


### Limitations
- It covers all functions (*that have at least 12 bytes*). So technically it can be used to 
replace any C function in a program since: `void noop(void) {}` is 15 bytes long.
- It works on Windows and Linux.

### How it works
It works by overwriting the first 12 bytes of the function with a jump to the target code. 
```asm
movabs rax, 0xdeadbeefdeadbeef
jmp rax
```


### How to use it
```c
//jump.h

/**
 * @brief Overwrites the first 12 bytes of a function with a jump to the target code.
 * @param at The function to overwrite.
 * @param to The target code (0xdeadbeefdeadbeef).
 */
int InsertJump(void* at, void* to);
```