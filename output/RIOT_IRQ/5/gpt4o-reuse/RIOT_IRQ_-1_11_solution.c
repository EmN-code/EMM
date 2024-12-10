
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second

static volatile int atomic_var = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;  // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Initialize the timer
    xtimer_t timer;
    xtimer_init(&timer);

    // Set up the interrupt handler
    irq_disable();  // Disable interrupts globally
    irq_set(0, irq_handler, NULL);  // Set the handler for interrupt 0
    irq_enable();  // Enable interrupts globally

    // Start the timer to trigger the interrupt
    xtimer_set(&timer, TIMER_INTERVAL);

    // Wait for the interrupt to occur
    while (atomic_var == 0) {
        xtimer_usleep(100000);  // Sleep for 100 ms
    }

    // Disable the interrupt
    irq_disable();

    printf("Interrupt handling test completed\n");

    return 0;
}
