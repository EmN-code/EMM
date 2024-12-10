
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

#define TIMER_INTERVAL (1000000U)  // 1 second

atomic_t interrupt_flag = ATOMIC_INIT(0);

void timer_callback(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    printf("Starting interrupt handling test...\n");

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Disable interrupts
        unsigned state = irq_disable();

        // Check the interrupt flag
        if (atomic_load(&interrupt_flag)) {
            printf("Interrupt occurred while interrupts were disabled.\n");
            atomic_store(&interrupt_flag, 0);
        }

        // Restore interrupts
        irq_restore(state);

        // Sleep for a short period to avoid busy-waiting
        xtimer_usleep(100000);  // 100 ms
    }

    return 0;
}
