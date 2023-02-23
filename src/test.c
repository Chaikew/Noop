#include <stdio.h>
#include "jump.h"

void hook(void) {
}

void original(void) {}

int main() {
    if (InsertJump(&original, &hook) == 0) {
        printf("Successfully hook original(void)!\n");
    } else {
        printf("Failed to hook original(void) :/ \n");
    }
    
    //int rv = original();
    //printf("original() returned %d\n", rv);
    return 0;
}