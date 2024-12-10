#include <stdio.h>
#include <irq.h>
#include <xtimer.h>

// Atomic variable to check interrupt status
volatile int interrupt_status = 0;

// Function to demonstrate interrupt handling
void handle_interrupt(void) {
    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the global interrupt_status
    interrupt_status++;

    // Print the current interrupt status
    printf("Interrupt status: %d\n", interrupt_status);

    // Restore interrupts
    irq_restore(state);
}

int main(void) {
    // Set a timer to call handle_interrupt every 2 seconds
    xtimer_ticks32_t interval = {2 * US_PER_SEC};

    while (1) {
        handle_interrupt();
        xtimer_periodic_wakeup(&xtimer_ticks32_now(), interval);
    }
    
    return 0;
}