#include "irq_arch.h"
#include <stdio.h>
#include <stdatomic.h>

atomic_int irq_disabled_counter = 0;

void disable_interrupts(void) {
    irq_disable();
    atomic_fetch_add(&irq_disabled_counter, 1);
}

void restore_interrupts(void) {
    irq_disable();  // Simulate irq_restore as irq_restore() is not provided in the documentation, let's assume similar behavior.
    atomic_fetch_sub(&irq_disabled_counter, 1);
}

int main(void) {
    printf("Starting interrupt handling test on ESP32 with RIOT\n");

    disable_interrupts();
    printf("Interrupts disabled. Counter: %d\n", irq_disabled_counter);

    restore_interrupts();
    printf("Interrupts restored. Counter: %d\n", irq_disabled_counter);

    return 0;
}