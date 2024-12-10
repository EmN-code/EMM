#include <stdio.h>  // for printf
#include <irq_arch.h>  // for irq_disable and irq_restore

volatile unsigned irq_state;
volatile int interrupt_enabled = 1;  // simulate atomic variable

void critical_section(void) {
    if (interrupt_enabled) {
        printf("Executing critical section with interrupts enabled\n");
    } else {
        printf("Executing critical section with interrupts disabled\n");
    }
}

int main(void) {
    // disable interrupts
    irq_state = irq_disable();
    interrupt_enabled = 0;  // simulate atomic operation
    
    // Execute code in critical section
    critical_section();
    
    // restore interrupts
    irq_restore(irq_state);
    interrupt_enabled = 1;  // simulate atomic operation
    
    // Execute code after restoring interrupts
    critical_section();

    return 0;
}