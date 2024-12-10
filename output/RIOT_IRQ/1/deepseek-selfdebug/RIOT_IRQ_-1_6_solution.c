
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h> // Include the correct header for atomic operations

volatile atomic_t interrupt_flag = ATOMIC_INIT(0); // Atomic variable to track interrupt state

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1); // Set the interrupt flag
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register the interrupt handler
    irq_disable(); // Disable interrupts globally
    irq_set_handler(0, interrupt_handler, NULL); // Set the handler for interrupt 0
    irq_enable(); // Enable interrupts globally

    // Simulate an interrupt by triggering the handler
    irq_trigger(0);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt was handled correctly
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
