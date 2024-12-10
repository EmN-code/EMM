
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U) // 1 second

static volatile int atomic_var = 0;

void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer callback triggered\n");
    atomic_var = 1; // Set atomic variable to 1
}

int main(void)
{
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    printf("Starting interrupt handling test\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set a timer to trigger in 1 second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Restore interrupts
    irq_restore(state);

    // Wait for the timer to trigger and set the atomic variable
    while (atomic_var == 0) {
        thread_yield();
    }

    printf("Atomic variable set to 1, interrupt handling works correctly\n");

    return 0;
}
