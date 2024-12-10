#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define INTERRUPT_PRIORITY 1

static atomic_uint32_t interrupt_counter = 0;

void IRAM_ATTR interrupt_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&interrupt_counter, 1);
    // Clear the interrupt (implementation-specific)
    // Example: intr_cntrl_ll_clear_interrupt(INTERRUPT_SOURCE);
}

void configure_interrupt(void)
{
    // Configure the interrupt source and priority (implementation-specific)
    // Example: intr_matrix_set(PRO_CPU_NUM, INTERRUPT_SOURCE, INTERRUPT_PRIORITY);
    // Set the interrupt handler
    intr_cntrl_ll_set_int_handler(INTERRUPT_PRIORITY, interrupt_handler, NULL);
    // Enable the interrupt
    intr_cntrl_ll_enable_interrupts(BIT(INTERRUPT_PRIORITY));
}

int main(void)
{
    printf("Starting RIOT on ESP32 with interrupt handling\n");

    // Configure and enable the interrupt
    configure_interrupt();

    // Main loop
    while (1) {
        // Disable interrupts
        uint32_t state = irq_disable();

        // Read the atomic variable
        uint32_t count = atomic_load(&interrupt_counter);

        // Restore interrupts
        irq_restore(state);

        // Print the interrupt counter
        printf("Interrupt counter: %u\n", count);

        // Sleep for a while
        xtimer_sleep(1);
    }

    return 0;
}