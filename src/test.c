#include <stdio.h>
#include "jump.h"

char* hook(void) {
    printf("Hooked called!\n");
    return "hook_result";
}

char* original(void) {
    printf("Original called!\n");
    return "original_result";
}

int main() {
    if (InsertJump(&original, &hook) == 0) {
        printf("Successfully hook original(void)!\n");
    } else {
        printf("Failed to hook original(void) :/ \n");
    }

    char* rv = original();
    printf("original() returned \"%s\"\n", rv);
    return 0;
}