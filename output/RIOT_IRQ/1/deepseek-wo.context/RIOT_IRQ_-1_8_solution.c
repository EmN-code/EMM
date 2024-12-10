
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_t counter = ATOMIC_INIT(0);  // Initialize atomic variable

void irq_handler(void *arg)
{
    (void)arg;
    atomic_add(&counter, 1);  // Increment the atomic counter
}

int main(void)
{
    puts("Starting interrupt handling test...");

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Trigger the IRQ to test the handler
    irq_trigger(IRQ_NUM);

    // Wait for a short period to ensure the IRQ handler has executed
    thread_sleep(100000);

    // Check the value of the atomic counter
    if (atomic_get(&counter) == 1) {
        puts("Interrupt handling test passed!");
    } else {
        puts("Interrupt handling test failed!");
    }

    return 0;
}
