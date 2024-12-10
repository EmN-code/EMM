
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "xtimer.h"
#include "thread.h"
#include "irq.h"

#define THREAD_STACKSIZE_DEFAULT (512)

static volatile atomic_int_t atomic_var = ATOMIC_INIT(0);

static void *interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt handler triggered\n");
    atomic_store(&atomic_var, 1);
    return NULL;
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Create a thread for the interrupt handler
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN, 0, interrupt_handler, NULL, "interrupt_handler");

    // Disable interrupts
    irq_disable();
    printf("Interrupts disabled\n");

    // Simulate some work
    xtimer_sleep(2);

    // Check the atomic variable (should be 0 since interrupts are disabled)
    if (atomic_load(&atomic_var) == 0) {
        printf("Atomic variable is 0, interrupts were disabled correctly\n");
    } else {
        printf("Atomic variable is not 0, interrupts were not disabled correctly\n");
    }

    // Enable interrupts
    irq_enable();
    printf("Interrupts enabled\n");

    // Simulate some work
    xtimer_sleep(2);

    // Check the atomic variable (should be 1 since interrupts are enabled)
    if (atomic_load(&atomic_var) == 1) {
        printf("Atomic variable is 1, interrupts were enabled correctly\n");
    } else {
        printf("Atomic variable is not 1, interrupts were not enabled correctly\n");
    }

    return 0;
}
