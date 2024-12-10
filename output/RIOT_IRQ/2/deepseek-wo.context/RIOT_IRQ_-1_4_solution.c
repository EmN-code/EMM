
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
    puts("Starting interrupt handling test...");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the interrupt
    irq_set(0, 1);

    // Sleep for a while to allow the interrupt to trigger
    thread_sleep();

    // Check the atomic variable
    if (atomic_load(&counter) == 1) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt was not handled.");
    }

    return 0;
}
