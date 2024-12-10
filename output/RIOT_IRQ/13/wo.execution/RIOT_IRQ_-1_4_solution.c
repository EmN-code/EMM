#include "time_units.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "unistd.h"
#include "netinet/in.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "irq_arch.h"
#include "irq.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "dht_params.h"

int main(void) {
    // Simulate an atomic variable
    atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

    // Disable all interrupts and store the previous state
    unsigned prev_state = irq_disable();
    printf("Interrupts disabled. Previous state stored: %u\n", prev_state);

    // Work with interrupts disabled
    atomic_store(&interrupt_flag, 1);
    printf("Atomic variable updated in critical section.\n");

    // Restore interrupts to previous state
    irq_restore(prev_state);
    printf("Interrupts restored to previous state: %u\n", prev_state);

    return 0;
}