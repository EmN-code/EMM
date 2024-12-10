#include "irq_arch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

atomic_int interrupt_count;

void critical_section(void) {
    unsigned state = irq_disable();
    atomic_fetch_add(&interrupt_count, 1);
    irq_restore(state);
}

int main(void) {
    // Initialize atomic variable
    atomic_init(&interrupt_count, 0);

    printf("Initial interrupt count: %d\n", atomic_load(&interrupt_count));

    // Simulate critical section access
    critical_section();

    printf("Interrupt count after critical section: %d\n", atomic_load(&interrupt_count));

    return 0;
}
