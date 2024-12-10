
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
    puts("Starting interrupt handling test...");

    // Set the IRQ handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the IRQ
    irq_set(0, 1);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Check the atomic variable
    if (atomic_load(&counter) == 1) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt was not handled.");
    }

    return 0;
}
