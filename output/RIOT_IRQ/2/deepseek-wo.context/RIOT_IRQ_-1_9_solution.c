
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_int_t counter;

void irq_handler(void *arg) {
    atomic_store(&counter, 1);
}

int main(void) {
    atomic_init(&counter, 0);

    // Set the IRQ handler
    irq_set_handler(irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    printf("Interrupts are disabled\n");

    // Restore interrupts
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Check if the interrupt handler was called
    int value = atomic_load(&counter);
    if (value == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
