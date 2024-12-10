#include <stdio.h>
#include <irq_arch.h>

volatile unsigned irq_status;
volatile unsigned global_count = 0;

void example_interrupt_handler(void) {
    // Critical section starts
    irq_status = irq_disable();

    // Perform some operations
    global_count++;

    // Restore interrupts
    irq_restore(irq_status);
    // Critical section ends
}

int main(void) {
    printf("Starting interrupt handling example on ESP32 using RIOT OS.\n");

    // Initial count before interrupt
    printf("Initial Count Value: %u\n", global_count);

    // Simulate an interrupt - in a real system, the handler would be triggered by hardware
    example_interrupt_handler();
    
    // Count after interrupt
    printf("Count Value after Interrupt: %u\n", global_count);

    return 0;
}