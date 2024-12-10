#include "stdio.h"
#include "unistd.h"
#include "stdatomic.h"
#include "stddef.h"
#include "irq_arch.h"
#include "irq.h"
#include "architecture.h"
#include "mma8x5x_regs.h"
#include "nimble/nimble_npl.h"

atomic_bool is_irq_enabled = ATOMIC_VAR_INIT(false);

void interrupt_handler(void) {
    // Simulating interrupt handling logic
    if (atomic_load(&is_irq_enabled)) {
        printf("Interrupt handled successfully.\n");
    }
    else {
        printf("Interrupt handling failed.\n");
    }
}

int main() {
    // Initially, assume interrupts are disabled.
    atomic_store(&is_irq_enabled, false);

    // Simulate enabling interrupts
    unsigned state = irq_enable();
    atomic_store(&is_irq_enabled, true);

    // Call the interrupt handler
    interrupt_handler();

    // Restore the previous IRQ state
    irq_restore(state);
    atomic_store(&is_irq_enabled, false);

    return 0;
}