#include <stdint.h>
#include <string.h>

#include "jump.h"

#if !__x86_64__
    /* not 64-bit */
    #error "This code is only for 64-bit systems :/"
#endif

static inline int __mprotect(void *__addr, size_t __len, int __prot);

#ifdef __linux__
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_EXECUTE_READWRITE PROT_EXEC | PROT_READ | PROT_WRITE
#define PAGE_EXECUTE_READ PROT_EXEC | PROT_READ

static inline int __mprotect(void *__addr, size_t __len, int __prot) {
    //  Find page size for this system.
    size_t pagesize = sysconf(_SC_PAGESIZE);

    //  Calculate start and end addresses for mprotect
    uintptr_t start = (uintptr_t) __addr;
    uintptr_t end = start + __len;

    //  Calculate start of page for mprotect.
    uintptr_t pagestart = start & -pagesize;

    // do mprotect
    return mprotect((void *) pagestart, end - pagestart, __prot);
}
#elif _WIN32
#include <memoryapi.h>
static inline int __mprotect(void *__addr, size_t __len, int __prot) {
    DWORD oldProtect;
    return !VirtualProtect((LPVOID)__addr, (SIZE_T )__len, (DWORD)__prot, &oldProtect);
}
#endif



int InsertJump(void* at, void* to) {
    #define JUMP_CODE_SIZE 12

    //  make it writable
    if (__mprotect(at, JUMP_CODE_SIZE, PAGE_EXECUTE_READWRITE)) {
        return -1;
    }

    // ===== jump insert start =====

    unsigned long long int jumpAddr = (uintptr_t)(to);

    // jumpAddr = 0xddccbbaadeadbeef
    char ef = (char)(jumpAddr & 0xFF);  // lowest byte
    char be = (char)(jumpAddr >> 8 & 0xFF);
    char ad = (char)(jumpAddr >> 16 & 0xFF);
    char de = (char)(jumpAddr >> 24 & 0xFF); // highest 32bit byte

    // 64bit
    char aa = (char)(jumpAddr >> 32 & 0xFF);
    char bb = (char)(jumpAddr >> 40 & 0xFF);
    char cc = (char)(jumpAddr >> 48 & 0xFF);
    char dd = (char)(jumpAddr >> 56 & 0xFF); // highest 64bit byte

    // https://defuse.ca/online-x86-assembler.htm#disassembly2

    // Research note:
    // we cant use "call rax", AFAIK it changes some registers
    // which will trow segfault instead we use "jmp rax"

    // Code understanding note:
    // a dummy address has been chosen to name
    // the address bytes in order to avoid confusion:
    // 0xddccbbaadeadbeef in 64bit (here)
    // 0xdeadbeef in 32bit (not here)

    // "jump" code using the dummy address:
    //
    // 0:  48 b8 ef be ad de aa    movabs rax,0xddccbbaadeadbeef
    // 7:  bb cc dd
    // a:  ff e0                   jmp   rax
    //

    size_t instructionsSize = JUMP_CODE_SIZE; // our jump code is 12 bytes long
    char instructions[JUMP_CODE_SIZE] = {
            (char)0x48, (char)0xb8, (char)ef, (char)be, (char)ad, (char)de, (char)aa, // movabs rax, 0x...
            (char)bb, (char)cc, (char)dd,
            (char)0xff, (char)0xe0,                     // jmp rax
    };

    // copy the instructions to the address (don't forget to mprotect to allow write)
    memcpy(at, instructions, instructionsSize);

    // ===== jump insert end =====



    //  make it not writable
    if (__mprotect(at, JUMP_CODE_SIZE, PAGE_EXECUTE_READ)) {
        return -1;
    }

    return 0;
}