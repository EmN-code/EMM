
#include "irq.h"
#include "stddef.h"
#include "net/sock/util.h"
#include "timex.h"
#include "xtimer.h"
#include "mma8x5x.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "msg.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "ztimer.h"
#include "time_units.h"
#include "evtimer_mbox.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdio.h"

atomic_int interrupt_count = ATOMIC_VAR_INIT(0);

void interrupt_handler(void)
{
    atomic_fetch_add(&interrupt_count, 1);
}

int main(void)
{
    unsigned state;

    // Disable interrupts
    state = irq_disable();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Restore interrupts
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Simulate an interrupt
    interrupt_handler();

    // Check the interrupt count
    int count = atomic_load(&interrupt_count);
    printf("Interrupt count: %d\n", count);

    return 0;
}
