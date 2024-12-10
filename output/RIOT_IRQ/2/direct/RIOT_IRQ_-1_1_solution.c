#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <irq.h>

atomic_bool irq_handled = ATOMIC_VAR_INIT(false);

void irq_handler(void) {
    // Simulate an interrupt being handled
    atomic_store(&irq_handled, true);
    printf("Interrupt handled.\n");
}

int main(void) {
    // Disable interrupts
    irq_disable();
    printf("Interrupts disabled.\n");

    // Check that the irq_handled flag is initially false
    if (atomic_load(&irq_handled) == false) {
        printf("Interrupt flag is correctly initialized to false.\n");
    }

    // Simulate handling an interrupt
    irq_handler();

    // Verify that interrupt flag is now true
    if (atomic_load(&irq_handled) == true) {
        printf("Interrupt flag set to true after handling an interrupt.\n");
    }

    // Restore interrupts
    irq_enable();
    printf("Interrupts enabled.\n");

    return 0;
}
