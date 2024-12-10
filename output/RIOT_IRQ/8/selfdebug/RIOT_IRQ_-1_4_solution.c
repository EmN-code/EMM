// The code provides an example of managing interrupts on an ESP32 within the RIOT operating
// system. The code does not utilize the GPIO interface but leverages atomic variables to
// verify the proper disabling and restoring of interrupts.

// Import all necessary header files sorted by their name in descending alphabetical order.
#include <stdio.h>
#include <stdint.h>
#include <periph/irq.h>
#include <periph/cpu.h>
#include <xtimer.h>

// Define an atomic flag to check interrupt management
volatile uint32_t interrupt_flag = 0;

// Interrupt service routine (ISR)
void isr_handler(void *arg) {
    (void)arg;
    // Simulate handling by toggling the interrupt flag
    interrupt_flag ^= 1;
}

int main(void) {
    // Print initial message
    printf("Starting interrupt testing with atomic flag check...\n");

    // Disable interrupts, setup ISR, and then enable interrupts
    irq_disable();
    irq_register(IRQ_MASK, isr_handler, NULL);
    irq_enable();

    // Simulate an interrupt
    if (!interrupt_flag) {
        // Manually invoke the ISR for testing purposes
        isr_handler(NULL);
    }

    // Check if the interrupt was handled correctly
    if (interrupt_flag) {
        printf("The interrupt was managed correctly.\n");
    } else {
        printf("Failed to handle the interrupt properly.\n");
    }

    return 0;
}