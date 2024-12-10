#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "irq_arch.h"

volatile bool irq_enabled = false;
volatile uint32_t irq_status;

void manage_interrupts(void) {
    irq_status = irq_disable();
    irq_enabled = irq_is_enabled();
    printf("Interrupts disabled: %d\n", irq_enabled);

    irq_restore(irq_status);
    irq_enabled = irq_is_enabled();
    printf("Interrupts restored: %d\n", irq_enabled);
}

int main(void) {
    printf("Starting interrupt management test...\n");

    manage_interrupts();

    printf("Test completed.\n");
    return 0;
}