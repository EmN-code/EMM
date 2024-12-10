#include <irq_arch.h>
#include <stdio.h>

volatile unsigned int interrupt_status = 0;

void handle_interrupt(void) {
    // Example interrupt handler
    printf("Interrupt occurred!\n");
    // Perform interrupt handling tasks here
    interrupt_status++; // Increment interrupt counter
}

int main(void) {
    printf("ESP32 RIOT Interrupt Handling Example\n");

    // Disable interrupts and store old state
    unsigned old_state = irq_disable();
    printf("Interrupts disabled\n");

    // Simulate some critical section of code
    for (volatile int i = 0; i < 10000; i++);

    // Restore interrupts to the previous state
    irq_restore(old_state);
    printf("Interrupts restored\n");

    // Simulate an interrupt event by directly calling the handler
    handle_interrupt();

    printf("Interrupt status: %u\n", interrupt_status);

    return 0;
}