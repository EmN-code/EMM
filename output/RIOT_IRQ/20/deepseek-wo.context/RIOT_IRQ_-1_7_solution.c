
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

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

    // Sleep for a short period to allow the interrupt to be handled
    thread_sleep();

    // Check the atomic variable to verify the interrupt was handled
    int result = atomic_load(&counter);
    if (result == 1) {
        printf("Interrupt was successfully handled\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}