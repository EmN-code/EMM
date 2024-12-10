
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the interrupt
    irq_enable(0);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Do nothing
    }

    // Check the counter value
    if (atomic_load(&counter) == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
