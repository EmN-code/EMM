
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define TIMER_INTERVAL (1000000U)  // 1 second

static volatile uint32_t atomic_var = 0;

void timer_callback(void *arg)
{
    (void)arg;
    atomic_var++;
    printf("Timer interrupt triggered, atomic_var: %u\n", atomic_var);
}

int main(void)
{
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Restore interrupts
    irq_restore(state);

    // Main loop
    while (1) {
        // Check the atomic variable
        if (atomic_var > 0) {
            printf("Atomic variable incremented, interrupts work correctly.\n");
            atomic_var = 0;  // Reset the atomic variable
        }
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
