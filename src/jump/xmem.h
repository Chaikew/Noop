#ifndef NOOP_XMEM_H
#define NOOP_XMEM_H

#include <stdint.h>
#include <stddef.h>

static inline int __mprotect(void *__addr, size_t __len, int __prot);

#if defined(__linux__)
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_EXECUTE_READWRITE (PROT_EXEC | PROT_READ | PROT_WRITE)
#define PAGE_EXECUTE_READ (PROT_EXEC | PROT_READ)

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
#elif defined(_WIN32)
#include <memoryapi.h>
static inline int __mprotect(void *__addr, size_t __len, int __prot) {
    DWORD oldProtect;
    return !VirtualProtect((LPVOID)__addr, (SIZE_T )__len, (DWORD)__prot, &oldProtect);
}
#endif

#endif //NOOP_XMEM_H
