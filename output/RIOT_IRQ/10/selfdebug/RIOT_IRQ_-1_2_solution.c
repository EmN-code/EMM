#include irq_arch.h

#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"

volatile unsigned irq_state;

void irq_disable_prototype(void) {
    irq_state = irq_disable();
}

void irq_restore_prototype(void) {
    irq_restore(irq_state);
}

int main(void) {
    ztimer_sleep(ZTIMER_MSEC, 1000);

    irq_disable_prototype();
    /* Critical section - interrupts are disabled */
    printf("Interrupts are disabled\n");

    /* Simulate work */
    xtimer_usleep(1000);

    irq_restore_prototype();
    /* End of critical section - interrupts are restored */
    printf("Interrupts are restored\n");

    return 0;
}