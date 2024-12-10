#include <irq_arch.h>
#include <stdio.h>

volatile unsigned irq_state = 0;
volatile atomic_bool interrupt_handling_successful = ATOMIC_VAR_INIT(false);

void test_disable_interrupt(void) {
    // Disable interrupts and save the state
    irq_state = irq_disable();
    printf("Interrupts disabled. State saved: %u\n", irq_state);
}

void test_restore_interrupt(void) {
    // Restore interrupt state
    irq_restore(irq_state);
    printf("Interrupts restored to state: %u\n", irq_state);
}

void perform_interrupt_test(void) {
    test_disable_interrupt();
    
    // Critical section: Perform operations that require interrupt to be disabled
    // For testing purposes, validate success of interrupt handling
    printf("Performing critical operations.\n");
    if (!irq_is_enabled()) {
        atomic_store(&interrupt_handling_successful, true);
    }
    
    test_restore_interrupt();
}

int main(void) {
    printf("Starting interrupt handling test.\n");
    perform_interrupt_test();
    
    if (atomic_load(&interrupt_handling_successful)) {
        printf("Interrupt handling was successful.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }
    
    return 0;
}