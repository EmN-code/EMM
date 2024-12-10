#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include <irq.h>

volatile uint32_t interrupt_counter = 0;

void handle_interrupt(void) {
    irq_disable();
    interrupt_counter++;
    irq_enable();
}

int main(void) {
    printf("Starting the interrupt handling program on ESP32\n");
    
    while (1) {
        // Simulating an interrupt event
        irq_disable();
        handle_interrupt();
        irq_enable();

        // Show the current count of handled interrupts
        printf("Interrupts handled: %lu\n", (unsigned long)interrupt_counter);

        // Wait 1 second before simulating the next interrupt.
        xtimer_sleep(1);
    }

    return 0;
}