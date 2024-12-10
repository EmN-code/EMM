
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define TIMER_INTERVAL (1000000U)  // 1 second

static volatile int atomic_var = 0;

void timer_callback(void *arg)
{
    (void)arg;
    atomic_var++;
    printf("Timer callback triggered, atomic_var: %d\n", atomic_var);
}

int main(void)
{
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();
    printf("Interrupts disabled, atomic_var: %d\n", atomic_var);

    // Set a timer to trigger an interrupt
    xtimer_set(&timer, TIMER_INTERVAL);

    // Simulate some work
    xtimer_usleep(2 * TIMER_INTERVAL);

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored, atomic_var: %d\n", atomic_var);

    // Wait for the timer to trigger
    xtimer_usleep(2 * TIMER_INTERVAL);

    printf("End of interrupt handling test.\n");

    return 0;
}
