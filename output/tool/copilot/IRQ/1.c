#include <stdio.h>
#include <stdint.h>
#include "irq.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define INTERRUPT_INTERVAL (1000000U) // 1 second

static volatile uint32_t interrupt_counter = 0;
static volatile uint32_t atomic_counter = 0;

void irq_handler(void)
{
    // Disable interrupts
    uint32_t state = irq_disable();

    // Increment the interrupt counter
    interrupt_counter++;

    // Increment the atomic counter atomically
    atomic_fetch_add_u32(&atomic_counter, 1);

    // Restore interrupts
    irq_restore(state);
}

void *thread_handler(void *arg)
{
    (void)arg;
    while (1) {
        // Print the counters every second
        printf("Interrupt counter: %u, Atomic counter: %u\n", interrupt_counter, atomic_counter);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    // Initialize the interrupt handler
    irq_enable();

    // Create a thread to print the counters
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread_handler");

    // Simulate interrupts
    while (1) {
        irq_handler();
        xtimer_usleep(INTERRUPT_INTERVAL);
    }

    return 0;
}