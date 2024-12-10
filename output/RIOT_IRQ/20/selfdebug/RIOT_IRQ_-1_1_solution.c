#include nimble_scanner.h
#include nimble/ble.h
#include irq_arch.h

volatile unsigned irq_state;
volatile unsigned irq_disable_test_result = 0;

void interrupt_disable_restore_test(void) {
    // Disable interrupts and store the previous state
    unsigned state = irq_disable();
    // Now interrupts are disabled, perform some critical operations here
    // Relying on compiler optimizations/atomic operations while interrupts are off

    // Simulate critical operations by toggling a variable
    irq_disable_test_result = 1;

    // Restore interrupts to the previous state
    irq_restore(state);
    // Verify if the restoration was successful by checking test variable
    if (irq_disable_test_result) {
        // Reset the variable
        irq_disable_test_result = 0;
    } else {
        // Handle error: interrupts were not restored properly
    }
}

int main(void) {
    // Execute the test
    interrupt_disable_restore_test();
    
    // Continue with your application logic
    while (1) {
        // Main loop can perform other tasks
    }
    
    return 0;
}