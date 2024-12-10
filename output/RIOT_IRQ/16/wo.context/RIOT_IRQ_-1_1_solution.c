#include <stdio.h>
#include <stdbool.h>
#include <irq_arch.h>
#include <stdatomic.h>

atomic_int interrupt_counter;

void handle_interrupt(void) {
    interrupt_counter++;
}

void test_interrupt_handling(void) {
    unsigned state;
    
    // Disable interrupts
    state = irq_disable();
    printf("Interrupts disabled. Current counter: %d\n", atomic_load(&interrupt_counter));

    // Simulate simple task
    for (int i = 0; i < 10; i++) {
        if (!irq_is_in()) {
            handle_interrupt();
        }
    }

    // Verify if interrupts are enabled
    if (!irq_is_enabled()) {
        printf("Interrupts are currently disabled, as expected.\n");
    }

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored. Current counter: %d\n", atomic_load(&interrupt_counter));
}

int main(void) {
    // Initialize the atomic counter
    atomic_init(&interrupt_counter, 0);
    
    // Test interrupt handling
    test_interrupt_handling();
    
    return 0;
}