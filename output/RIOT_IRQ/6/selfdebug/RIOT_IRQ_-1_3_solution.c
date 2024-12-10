#include <stdio.h>
#include <stdatomic.h>
#include "irq_arch.h"  // Necessary for irq functions

volatile atomic_int interrupt_flag = 1; // atomic variable to check interrupt handling

void custom_function(void) {
    // Dummy function to simulate some processing
    for (volatile int i = 0; i < 1000; i++);
}

int main(void) {
    // Informing the start of the program
    printf("Starting program...\n");

    // Disable interrupts
    irq_disable();
    printf("Interrupts disabled.\n");
    
    // Update atomic variable when interrupts are disabled
    atomic_store(&interrupt_flag, 0);

    // Call some function
    custom_function();

    // Restore interrupts
    irq_restore();
    printf("Interrupts restored.\n");

    // Verify changes
    if (atomic_load(&interrupt_flag) == 0) {
        printf("Atomic variable correctly updated with interrupts disabled.\n");
    } else {
        printf("Atomic variable update failed.\n");
    }

    return 0;
}
