#include <stdio.h>
#include <irq_arch.h>

unsigned int irq_restore_state;
volatile int shared_counter = 0;

void critical_section() {
    irq_restore_state = irq_disable();
    shared_counter++;
    irq_restore(irq_restore_state);
}

int main(void) {
    printf("Starting interrupt handling example with atomic operations.\n");

    int expected_value = 10;
    for (int i = 0; i < expected_value; i++) {
        critical_section();
    }

    if (shared_counter == expected_value) {
        printf("Interrupt handling and atomic operation are working correctly. Counter: %d\n", shared_counter);
    } else {
        printf("Mismatch in counter. Expected: %d, but got: %d\n", expected_value, shared_counter);
    }

    return 0;
}