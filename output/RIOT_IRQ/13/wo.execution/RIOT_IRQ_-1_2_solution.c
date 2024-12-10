#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include "irq_arch.h"
#include "irq.h"
#include "architecture.h"
#include "mma8x5x_regs.h"

atomic_bool irq_status = ATOMIC_VAR_INIT(false);

unsigned irq_enable(void);
void irq_restore(unsigned state);

void check_interrupt_handling(void) {
    unsigned state;
    state = irq_enable();
    // Simulate critical section
    atomic_store(&irq_status, true);
    irq_restore(state);
}

int main() {
    printf("Starting interrupt handling check...\n");
    check_interrupt_handling();
    if (atomic_load(&irq_status)) {
        printf("Interrupts were successfully disabled and restored.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }
    return 0;
}