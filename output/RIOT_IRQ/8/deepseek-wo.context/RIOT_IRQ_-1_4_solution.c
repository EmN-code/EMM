
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_int_t atomic_var = ATOMIC_INIT(0);  // Correctly initialize the atomic variable

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);  // Store 1 into the atomic variable
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Trigger the IRQ to simulate an interrupt
    irq_trigger(IRQ_NUM);

    // Load the value from the atomic variable
    int value = atomic_load(&atomic_var);

    // Check if the interrupt was handled correctly
    if (value == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}
