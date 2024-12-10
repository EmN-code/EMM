
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
    atomic_var = 1; // Set atomic variable
}

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt handler triggered\n");
    atomic_var = 0; // Reset atomic variable
}

int main(void)
{
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set up the interrupt handler
    irq_set_handler(0, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Restore interrupts
    irq_restore(state);

    // Wait for the timer to trigger
    while (atomic_var == 0) {
        xtimer_usleep(100000); // Sleep for 100 ms
    }

    printf("Atomic variable set to: %d\n", atomic_var);

    return 0;
}
