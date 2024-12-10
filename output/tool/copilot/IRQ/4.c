#include <stdio.h>
#include <stdint.h>
#include <stdatomic.h>
#include "irq.h"
#include "thread.h"

// Define atomic variables
static atomic_uint_fast32_t interrupt_counter;
static atomic_bool interrupt_flag;

// Interrupt handler function
void IRAM_ATTR my_interrupt_handler(void)
{
    // Increment the interrupt counter
    atomic_fetch_add(&interrupt_counter, 1);
    // Set the interrupt flag
    atomic_store(&interrupt_flag, true);
}

// Function to enable the interrupt
void enable_interrupt(void)
{
    // Register the interrupt handler
    irq_enable();
    // Enable the interrupt source (example: timer interrupt)
    // Note: Replace with actual interrupt source initialization
    // timer_init();
}

// Function to disable the interrupt
void disable_interrupt(void)
{
    // Disable the interrupt source (example: timer interrupt)
    // Note: Replace with actual interrupt source deinitialization
    // timer_deinit();
    irq_disable();
}

int main(void)
{
    // Initialize atomic variables
    atomic_init(&interrupt_counter, 0);
    atomic_init(&interrupt_flag, false);

    // Enable the interrupt
    enable_interrupt();

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        // Disable interrupts
        uint32_t state = irq_disable();
        // Critical section
        printf("Critical section: %d\n", i);
        // Restore interrupts
        irq_restore(state);
    }

    // Disable the interrupt
    disable_interrupt();

    // Check the interrupt counter and flag
    printf("Interrupt counter: %u\n", atomic_load(&interrupt_counter));
    printf("Interrupt flag: %d\n", atomic_load(&interrupt_flag));

    return 0;
}