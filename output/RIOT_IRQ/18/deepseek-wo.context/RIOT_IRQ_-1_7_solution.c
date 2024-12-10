
#include <stdio.h>
#include <stdint.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define TIMER_INTERVAL (1000000U)  // 1 second

volatile uint32_t atomic_var = 0;

void timer_callback(void *arg)
{
    (void)arg;
    atomic_var++;
}

int main(void)
{
    xtimer_t timer;

    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_var = 10;

    // Restore interrupts
    irq_restore(state);

    // Wait for the timer to trigger
    xtimer_usleep(TIMER_INTERVAL * 2);

    // Check the value of the atomic variable
    if (atomic_var == 11) {
        printf("Interrupts were correctly disabled and restored.\n");
    } else {
        printf("Interrupts were not correctly disabled and restored.\n");
    }

    return 0;
}
