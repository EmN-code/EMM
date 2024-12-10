#include <irq_arch.h>
#include <inttypes.h>

volatile uint32_t interrupt_status = 0;

static inline void test_interrupt_handling(void) {
    // Disable interrupts and store the state
    unsigned status = irq_disable();
    // If interrupts were enabled, set the interrupt_status atomic variable
    if (status) {
        interrupt_status = 1;
    }
    // Simulate some operation
    for (volatile int i = 0; i < 1000000; ++i);
    // Restore interrupts
    irq_restore(status);
    // Check if interrupts are restored
    if (irq_is_enabled()) {
        interrupt_status = 2;
    }
}

int main(void) {
    // Initialize interrupt_status
    interrupt_status = 0;
    
    // Test the interrupt handling
    test_interrupt_handling();

    // Print result
    if (interrupt_status == 0) {
        printf("Interrupts were never disabled.\n");
    } else if (interrupt_status == 1) {
        printf("Interrupts were disabled and restored successfully.\n");
    } else if (interrupt_status == 2) {
        printf("Interrupts were restored correctly.\n");
    }

    return 0;
}