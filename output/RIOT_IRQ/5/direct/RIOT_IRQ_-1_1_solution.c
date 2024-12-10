#include <stdio.h>
#include <stdatomic.h>
#include <irq.h>

// Initialize an atomic variable to verify interrupt disable/restore
atomic_bool interrupt_handled = ATOMIC_VAR_INIT(false);

// Interrupt handler function
void esp32_isr(void) {
    // Set atomic variable to true to indicate ISR execution
    atomic_store(&interrupt_handled, true);
    printf("Interrupt handled!\n");
}

int main(void) {
    // Disable interrupts
    irq_disable();
    printf("Interrupts disabled.\n");

    // Simulate external event triggering an interrupt
    // In an actual application, this would be triggered by hardware
    esp32_isr();

    // Check if the interrupt was handled during disabled state
    if (atomic_load(&interrupt_handled)) {
        printf("Interrupt was handled while interrupts were disabled.\n");
    } else {
        printf("Interrupt was not handled as expected.\n");
    }

    // Re-enable interrupts
    irq_enable();
    printf("Interrupts enabled.\n");

    // Reset atomic variable
    atomic_store(&interrupt_handled, false);

    // Simulate another interrupt now that interrupts are enabled
    esp32_isr();

    // Check again if the interrupt was handled
    if (atomic_load(&interrupt_handled)) {
        printf("Interrupt was handled after re-enabling interrupts.\n");
    } else {
        printf("Interrupt was not handled after enabling interrupts.\n");
    }

    return 0;
}