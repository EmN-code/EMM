
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second

static volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented to: %d\n", atomic_var);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the timer
    xtimer_t timer;
    xtimer_init(&timer);

    // Set up the interrupt handler
    irq_disable();
    irq_set(TIMER_IRQ, irq_handler, NULL);
    irq_enable();

    // Start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop to check the atomic variable
    while (1) {
        if (atomic_var > 0) {
            printf("Atomic variable is greater than 0, disabling interrupts...\n");
            irq_disable();
            xtimer_remove(&timer);
            printf("Interrupts disabled, exiting...\n");
            break;
        }
        xtimer_usleep(100 * 1000);  // Sleep for 100 ms
    }

    return 0;
}
