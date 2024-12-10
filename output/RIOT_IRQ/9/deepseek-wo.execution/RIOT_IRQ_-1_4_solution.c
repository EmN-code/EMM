
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <unistd.h>
#include <xtimer.h>
#include <irq.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to handle the interrupt
void irq_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(ESP32_IRQ_NUM, irq_handler);

    // Enable the interrupt
    irq_enable(ESP32_IRQ_NUM);

    // Restore the interrupt state
    irq_restore(state);

    // Simulate some work
    xtimer_sleep(1);

    // Check the atomic variable
    if (atomic_load(&atomic_var) > 0) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
