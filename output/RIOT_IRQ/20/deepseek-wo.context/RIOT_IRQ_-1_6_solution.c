
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_int counter = ATOMIC_VAR_INIT(0);  // Initialize atomic variable

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);  // Store 1 into the atomic variable
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Trigger the IRQ to simulate an interrupt
    irq_trigger(IRQ_NUM);

    // Wait for a short period to allow the interrupt to be handled
    thread_sleep(100000);

    // Load the value from the atomic variable
    int value = atomic_load(&counter);

    // Check if the interrupt was handled correctly
    if (value == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}
