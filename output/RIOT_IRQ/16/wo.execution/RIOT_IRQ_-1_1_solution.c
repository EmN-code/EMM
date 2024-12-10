#include <irq_arch.h>
#include <stdio.h>

volatile bool interrupt_enabled = false;

void toggle_irq(void) {
    if (interrupt_enabled) {
        irq_disable();
        printf("Interrupts disabled\n");
    } else {
        irq_is_enabled();
        printf("Interrupts enabled\n");
    }
    interrupt_enabled = !interrupt_enabled;
}

int main(void) {
    printf("Initial state: %s\n", irq_is_enabled() ? "enabled" : "disabled");
    
    // Toggle interrupts a few times to see results
    for (int i = 0; i < 5; i++) {
        toggle_irq();
    }

    return 0;
}