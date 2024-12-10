
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <irq.h>
#include <atomic_utils.h>

#define TIMER_INTERVAL (1000000U)  // 1 second

static volatile atomic_t flag = ATOMIC_INIT(0);

void timer_callback(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);
    printf("Interrupt triggered, flag set to 1\n");
}

int main(void)
{
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    printf("Starting interrupt handling test\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the timer to trigger an interrupt
    xtimer_set(&timer, TIMER_INTERVAL);

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to trigger
    while (atomic_load(&flag) == 0) {
        xtimer_usleep(10000);  // Sleep for 10 ms
    }

    printf("Interrupt handled, flag is now: %d\n", atomic_load(&flag));

    return 0;
}
