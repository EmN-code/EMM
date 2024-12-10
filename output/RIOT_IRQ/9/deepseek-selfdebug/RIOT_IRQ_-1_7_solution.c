
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

volatile atomic_t counter = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register an interrupt handler
    irq_disable();
    irq_set_handler(0, interrupt_handler, NULL);
    irq_enable();

    // Simulate an interrupt
    irq_set(0);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt was handled correctly
    if (atomic_load(&counter) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled correctly.\n");
    }

    return 0;
}
