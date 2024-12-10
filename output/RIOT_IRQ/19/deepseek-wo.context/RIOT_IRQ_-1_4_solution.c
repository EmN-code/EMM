
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

// Define the interrupt number to use
#define IRQ_NUM 0

// Define the atomic variable
atomic_int_t atomic_var = ATOMIC_INIT(0);

// Interrupt handler function
void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    // Initialize the atomic variable
    atomic_store(&atomic_var, 0);

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Load the value of the atomic variable and print it
    int value = atomic_load(&atomic_var);
    printf("Atomic variable value: %d\n", value);

    return 0;
}
