#include <irq_arch.h>
#include <stdbool.h>  // for boolean type
#include <stdio.h>    // for standard input-output

volatile bool interrupt_status;

void disable_interrupts_and_verify(void) {
    unsigned state = irq_disable();
    // Critical section: check if interrupts are disabled
    interrupt_status = !irq_is_enabled();
    irq_enable(state);
}

int main() {
    // Initial interrupt status
    printf("Initial interrupt status: %s\n", irq_is_enabled() ? "enabled" : "disabled");
    
    // Verify disable
    disable_interrupts_and_verify();
    
    if (interrupt_status) {
        printf("Interrupts were successfully disabled and verified.\n");
    } else {
        printf("Interrupt disabling verification failed.\n");
    }

    // Final interrupt status
    printf("Final interrupt status: %s\n", irq_is_enabled() ? "enabled" : "disabled");

    return 0;
}