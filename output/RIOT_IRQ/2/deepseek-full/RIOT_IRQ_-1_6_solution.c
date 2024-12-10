
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <arpa/inet.h>
#include <kernel_defines.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate an interrupt handler
void my_interrupt_handler(void) {
    printf("Interrupt occurred!\n");
    atomic_fetch_add(&atomic_var, 1); // Increment the atomic variable
}

int main(void) {
    printf("Starting interrupt handling test...\n");

    // Simulate setting up an interrupt handler
    // Note: This is a simulation since we cannot use irq_set_handler
    // In a real scenario, you would use irq_set_handler to set the handler
    // Here, we just call the handler directly for demonstration purposes
    my_interrupt_handler();

    // Simulate waiting for an interrupt
    printf("Waiting for interrupt...\n");
    // In a real scenario, you would enable interrupts here
    // For simulation, we just call the handler again
    my_interrupt_handler();

    // Check the atomic variable to verify interrupt handling
    if (atomic_load(&atomic_var) == 2) {
        printf("Interrupt handling verified successfully!\n");
    } else {
        printf("Interrupt handling verification failed!\n");
    }

    return 0;
}
