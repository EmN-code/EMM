#include "lwip/pbuf.h"
#include "irq_arch.h"

#include <stdbool.h>
#include <stdio.h>

volatile bool interrupt_occurred = false;

void irq_handler(void) {
    unsigned state;

    // Disable interrupts
    state = irq_disable();

    // Simulate handling an interrupt
    interrupt_occurred = true;

    printf("Interrupt handled. \n");

    // Restore interrupts
    irq_restore(state);
}

int main(void) {
    unsigned state;

    printf("Starting interrupt handling demo...\n");

    // Simulate an interrupt occurring
    irq_handler();

    // Disable interrupts globally
    state = irq_disable();

    if (interrupt_occurred) {
        printf("Interrupt handling verified. \n");
    } else {
        printf("Interrupt not occurred. Fail to handle!\n");
    }

    // Restore interrupts globally
    irq_restore(state);

    return 0;
}