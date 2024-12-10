#include "irq.h"
#include "xtimer.h"
#include <stdio.h>

volatile uint32_t interrupt_counter = 0;

void my_interrupt_handler(void *arg) {
    (void)arg;
    interrupt_counter++;
    printf("Interrupt occurred! Counter: %u\n", interrupt_counter);
}

int main(void) {
    printf("Starting interrupt handling example\n");

    // Disable interrupts and save the current state
    unsigned int state = irq_disable();

    // Set up your interrupt handler here
    // For demonstration, we simulate an interrupt using a timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 1000000); // 1 second
        my_interrupt_handler(NULL); // Simulate an interrupt
    }

    // Restore the interrupt state
    irq_restore(state);

    return 0;
}