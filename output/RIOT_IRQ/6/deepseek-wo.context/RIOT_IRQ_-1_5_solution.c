
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void *arg)
{
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Trigger the interrupt
    irq_trigger(0);

    // Wait for the interrupt to be handled
    thread_sleep();

    // Check the atomic variable
    int value = atomic_load(&counter);
    if (value == 1) {
        printf("Interrupt handled correctly\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}
