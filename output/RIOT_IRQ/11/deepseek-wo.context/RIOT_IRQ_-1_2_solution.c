
#include <stdio.h>
#include <stdint.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_NUM 0  // Define the IRQ number to use

volatile uint32_t atomic_var = 0;  // Use uint32_t instead of atomic_t

void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;  // Set the atomic variable to 1
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the atomic variable
    atomic_var = 0;

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Enable the IRQ
    irq_enable(IRQ_NUM);

    // Simulate an interrupt by setting the IRQ
    irq_set(IRQ_NUM);

    // Wait for a short period to allow the interrupt to be handled
    xtimer_sleep(1);

    // Check the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled correctly.\n");
    }

    return 0;
}
