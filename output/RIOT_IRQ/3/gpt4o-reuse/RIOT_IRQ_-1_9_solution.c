
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second

static volatile int atomic_var = 0;

void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer callback triggered\n");
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
}

int main(void)
{
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    printf("Starting interrupt handling test\n");

    // Initialize and start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some work with interrupts disabled
    printf("Interrupts disabled, simulating work...\n");
    xtimer_usleep(500 * 1000);  // Sleep for 500 ms

    // Restore interrupts
    irq_restore(state);

    // Wait for the timer interrupt to occur
    while (atomic_var == 0) {
        xtimer_usleep(10 * 1000);  // Sleep for 10 ms
    }

    printf("Interrupt occurred, atomic variable set to %d\n", atomic_var);

    // Clean up
    xtimer_remove(&timer);

    return 0;
}
