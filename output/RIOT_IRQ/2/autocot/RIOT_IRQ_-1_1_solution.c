#include <irq_arch.h>
#include "periph/irq.h"
#include <inttypes.h>

atomic_bool interrupts_disabled;

typedef void (*handler_fn)(void);

void example_interrupt_handler(void) {
    if (irq_is_in()) {
        printf("Inside interrupt handler!\n");
    }
}

void disable_interrupts(void) {
    unsigned state = irq_disable();
    interrupts_disabled = true;
    irq_restore(state);
}

int main(void) {
    printf("Starting Program\n");

    // Initialize the atomic flag
    atomic_init(&interrupts_disabled, false);
    
    // Simulate interrupt handling
    handler_fn my_handler = example_interrupt_handler;
    
    if (!atomic_load(&interrupts_disabled)) {
        disable_interrupts();
    }
    
    if (irq_is_enabled()) {
        printf("Interrupts are enabled\n");
    } else {
        printf("Interrupts are disabled\n");
    }

    // Call the interrupt handler
    my_handler();

    return 0;
}