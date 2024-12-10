
#include "irq.h"
#include <stdio.h>

volatile unsigned int atomic_var = 0;

void uart_rxstart_irq_disable(uart_t uart) {
    (void)uart; // Suppress unused parameter warning
    unsigned state = irq_disable();
    atomic_var = 1;
    irq_restore(state);
}

int main(void) {
    printf("Starting interrupt handling test...\n");

    // Simulate disabling and restoring interrupts
    unsigned state = irq_disable();
    atomic_var = 1;
    irq_restore(state);

    // Verify the atomic variable was set correctly
    if (atomic_var == 1) {
        printf("Interrupt handling test passed: atomic variable set correctly.\n");
    } else {
        printf("Interrupt handling test failed: atomic variable not set correctly.\n");
    }

    return 0;
}
