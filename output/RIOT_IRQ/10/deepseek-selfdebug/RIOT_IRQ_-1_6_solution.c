
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Disable interrupts
    irq_disable();

    // Check the atomic variable
    if (atomic_load(&counter) == 1) {
        printf("Interrupts were correctly disabled and restored.\n");
    } else {
        printf("Interrupts were not correctly disabled and restored.\n");
    }

    return 0;
}
