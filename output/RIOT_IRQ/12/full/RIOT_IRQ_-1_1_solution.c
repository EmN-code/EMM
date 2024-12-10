#include "ztimer.h"
#include "irq.h"
#include "xtimer.h"
#include <stdatomic.h>
#include <stdio.h>

atomic_int irq_counter;

void test_irq_handling(void) {
    unsigned state = irq_disable();

    // Critical section
    atomic_fetch_add(&irq_counter, 1);
    printf("IRQ disabled, counter incremented to %d\n", atomic_load(&irq_counter));

    irq_restore(state);

    // After restoring interrupts, check counter increment
    printf("IRQ restored, final counter value is %d\n", atomic_load(&irq_counter));
}

int main(void) {
    atomic_init(&irq_counter, 0);
    for (int i = 0; i < 5; i++) {
        test_irq_handling();
        xtimer_sleep(1);
    }
    return 0;
}