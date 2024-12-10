
#include "xtimer.h"
#include "irq.h"
#include "thread.h"
#include "atomic_utils.h"

#define TIMER_INTERVAL (1000000U)  // 1 second

static volatile atomic_t atomic_var = ATOMIC_INIT(0);

void timer_callback(void *arg)
{
    (void)arg;

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_store(&atomic_var, 1);

    // Restore interrupts
    irq_restore(state);
}

int main(void)
{
    xtimer_t timer;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer callback
    xtimer_set_cb(&timer, timer_callback, NULL);

    // Start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Check the atomic variable
        if (atomic_load(&atomic_var) == 1) {
            printf("Interrupt handling verified: atomic variable set to 1\n");
            atomic_store(&atomic_var, 0);
        }

        // Sleep for a while to avoid busy-waiting
        xtimer_usleep(100000);  // Sleep for 100 ms
    }

    return 0;
}
