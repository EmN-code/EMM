
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <xtimer.h>

#define IRQ_NUM 0  // Define the IRQ number to use

volatile uint32_t atomic_var = 0;

void irq_handler(void *arg)
{
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented to %u\n", atomic_var);
}

int main(void)
{
    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Enable the IRQ
    irq_enable(IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Main loop iteration %d\n", i);
    }

    // Disable the IRQ
    irq_disable(IRQ_NUM);

    // Verify the atomic variable
    if (atomic_var > 0) {
        printf("Interrupts were correctly handled and restored. atomic_var = %u\n", atomic_var);
    } else {
        printf("Interrupts were not handled correctly. atomic_var = %u\n", atomic_var);
    }

    return 0;
}
