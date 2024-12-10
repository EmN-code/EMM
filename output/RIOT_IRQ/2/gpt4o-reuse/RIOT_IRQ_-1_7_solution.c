
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds

static volatile int atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    unsigned state;

    // Initialize the timer
    xtimer_t timer;
    xtimer_init(&timer, TIMER_INTERVAL);

    // Register the interrupt handler
    irq_set_handler(TIMER_IRQ, interrupt_handler, NULL);

    // Disable interrupts
    state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_var = 0;

    // Restore interrupts to the previous state
    irq_restore(state);

    // Enable the timer interrupt
    xtimer_set(&timer, TIMER_INTERVAL);

    // Wait for the interrupt to occur
    while (atomic_var == 0) {
        xtimer_usleep(10000);  // Sleep for 10ms
    }

    // Disable the timer interrupt
    xtimer_unset(&timer);

    // Disable interrupts again
    state = irq_disable();

    // Verify the atomic variable
    if (atomic_var == 1) {
        printf("Interrupts were correctly disabled and restored\n");
    } else {
        printf("Interrupts were not correctly disabled and restored\n");
    }

    // Restore interrupts to the previous state
    irq_restore(state);

    return 0;
}
