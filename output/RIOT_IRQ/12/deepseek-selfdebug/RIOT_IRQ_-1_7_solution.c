
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Enable the interrupt
    irq_set(IRQ_NUM, 1);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Check the atomic variable
    if (atomic_load(&counter) == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
