## Acrobatic Overwriting
This repo contains some C x86/x64 trash code that can be used to "replace C functions".

### Limitations
- #### x64 (Windows/Linux)
  - The jump code is 12 bytes long.
  - It covers all functions (*that have at least 12 bytes*). So technically it can be used to replace any C function in a program since: `void noop(void) {}` is 15 bytes long.
- #### x86 (Windows/Linux)
  - The jump code is 7 bytes long.
  - It covers all functions (*that have at least 7 bytes*). So technically it can be used to replace any C function in a program since: `void noop(void) {}` is 20 bytes long.
-  Other platforms are not supported.

### How it works
It works by overwriting the first __"jump code size"__ bytes of the function with a jump to the target code.
```asm
// x64
movabs rax, 0xdeadbeefdeadbeef
jmp rax

// x86
mov eax, 0xdeadbeef
jmp eax
```

### How to use it
```c
//jump.h

/**
 * @brief Overwrites the first few bytes of a function with a jump to the target code.
 * @param at The function to overwrite.
 * @param to The target code (0xdeadbeefdeadbeef).
 */
int InsertJump(void* at, void* to);
```