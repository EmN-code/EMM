#include <xtensa/hal.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdint.h>
#include <inttypes.h>

atomic_int interrupt_status = ATOMIC_VAR_INIT(0);

void my_interrupt_handler(void) {
    atomic_fetch_add(&interrupt_status, 1);
    printf("Interrupt handled, status: %" PRIu32 "\n", atomic_load(&interrupt_status));
}

int main(void) {
    printf("Setting up interrupt handling example...\n");

    irqstate_t state = irq_disable();

    // Simulate interrupt occurrence
    my_interrupt_handler();

    irq_restore(state);

    printf("Exiting the interrupt handling example. Final status: %" PRIu32 "\n", atomic_load(&interrupt_status));
    return 0;
}