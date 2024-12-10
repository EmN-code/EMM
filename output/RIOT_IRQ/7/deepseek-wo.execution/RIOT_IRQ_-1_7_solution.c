
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <unistd.h>
#include <stddef.h>

// Define the UART IRQ number for ESP32
#define UART_IRQ_NUM 4

// Global atomic variable to verify interrupt handling
atomic_int interrupt_count = ATOMIC_VAR_INIT(0);

// Interrupt handler function
void my_interrupt_handler(void)
{
    atomic_fetch_add(&interrupt_count, 1);
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set the exclusive interrupt handler for UART IRQ
    irq_set_exclusive_handler(UART_IRQ_NUM, my_interrupt_handler);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Interrupt count: %d\n", atomic_load(&interrupt_count));
        sleep(1);
    }

    // Restore the interrupt state
    irq_restore(state);

    return 0;
}
