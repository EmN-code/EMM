#include <thread_arch.h>    // Required for thread management functions
#include <timex.h>           // Required for time-related functions and structures
#include <timer_arch.h>      // Required for timer functions
#include <stdio.h>           // For printing output
#include <periph_cpu.h>      // Required for peripheral control, including interrupts
#include <irq_arch.h>        // Required for irq_enable

volatile int atomic_var = 0;

void check_interrupt_handling(void) {
    unsigned state;

    // Disable interrupts and save the current state
    state = irq_disable();

    // Perform operations on the atomic variable
    atomic_var++;

    // Check the atomic variable
    if (atomic_var > 0) {
        printf("Interrupts disabled successfully. Atomic variable: %d\n", atomic_var);
    }

    // Restore the state of interrupts
    irq_restore(state);

    // Verify if interrupts are enabled
    printf("Interrupts restored.\n");
}

int main(void) {
    puts("RIOT ESP32 Interrupt Handling Example");

    // Call the function to check interrupt handling
    check_interrupt_handling();

    while (1) {
        // Main loop can continue executing other tasks
    }
    return 0;
}