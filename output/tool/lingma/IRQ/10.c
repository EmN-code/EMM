#include "xtimer.h"
#include "irq.h"
#include "atomic.h"
#include "periph/adc.h"
#include "periph/timer.h"

// Define an atomic variable to track the interrupt status
atomic_t interrupt_status = ATOMIC_INIT(0);

// Function to handle the timer interrupt
void timer_callback(void *arg) {
    (void)arg;

    // Atomically increment the interrupt status
    atomic_fetch_add(&interrupt_status, 1);

    // Print the interrupt status
    printf("Interrupt triggered, status: %u\n", atomic_load(&interrupt_status));
}

int main(void) {
    // Initialize the timer
    timer_t timer;
    TimerInit(&timer);

    // Set up the timer to trigger an interrupt every 1 second
    TimerCountdownMS(&timer, 1000);

    // Register the timer callback
    timer_set_callback(&timer, timer_callback, NULL);

    // Start the timer
    timer_start(&timer);

    while (1) {
        // Disable interrupts
        irq_disable();

        // Perform some critical section operations
        printf("Critical section: Interrupts disabled\n");

        // Simulate some work
        xtimer_usleep(100000); // Sleep for 100ms

        // Restore interrupts
        irq_restore();

        // Perform some non-critical section operations
        printf("Non-critical section: Interrupts restored\n");

        // Simulate some work
        xtimer_usleep(100000); // Sleep for 100ms
    }

    return 0;
}