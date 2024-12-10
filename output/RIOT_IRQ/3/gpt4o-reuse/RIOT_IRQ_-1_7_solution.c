
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static volatile uint32_t atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_var = 1;
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register the interrupt handler
    irq_set(0, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_var = 0;

    // Restore interrupts
    irq_restore(state);

    // Simulate some work
    xtimer_sleep(1);

    // Check the atomic variable
    if (atomic_var == 0) {
        printf("Interrupts were correctly disabled and restored.\n");
    } else {
        printf("Interrupts were not correctly disabled and restored.\n");
    }

    return 0;
}
