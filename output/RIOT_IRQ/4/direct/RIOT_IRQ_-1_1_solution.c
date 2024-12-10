#include <xtimer.h>
#include <stdio.h>
#include <stdatomic.h>
#include <irq.h>

atomic_bool interrupt_occurred = false;

void handle_interrupt(void) {
    atomic_store(&interrupt_occurred, true);
    printf("Interrupt occurred!\n");
}

int main(void) {
    printf("Interrupt Handling Example on RIOT with ESP32\n");
    
    unsigned state = irq_disable();  // Disable interrupts
    
    handle_interrupt(); // Simulated interrupt handling

    if (atomic_load(&interrupt_occurred)) {
        printf("Interruption was successfully handled.\n");
    }

    irq_restore(state); // Restore interrupts

    if (!atomic_load(&interrupt_occurred)) {
        printf("Interrupts successfully restored and no new interrupt occurred.\n");
    }

    return 0;
}