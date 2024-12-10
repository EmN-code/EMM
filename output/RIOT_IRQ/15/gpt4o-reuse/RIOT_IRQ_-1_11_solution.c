
#include <stdio.h>
#include <xtimer.h>
#include <irq.h>
#include <thread.h>

#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds

static volatile uint32_t atomic_var = 0;

void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer callback triggered\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify atomic variable
    atomic_var++;

    // Restore interrupts
    irq_restore(state);

    // Re-arm the timer
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void)
{
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop to check the atomic variable
    while (1) {
        // Disable interrupts
        unsigned state = irq_disable();

        // Check the atomic variable
        if (atomic_var > 0) {
            printf("Atomic variable modified: %lu\n", (unsigned long)atomic_var);
            atomic_var = 0;  // Reset the atomic variable
        }

        // Restore interrupts
        irq_restore(state);

        // Sleep for a while to avoid busy-waiting
        xtimer_usleep(100000);  // Sleep for 100ms
    }

    return 0;
}
