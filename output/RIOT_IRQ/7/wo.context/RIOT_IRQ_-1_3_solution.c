#include <xtimer.h>
#include <thread.h>
#include <stdint.h>
#include <stdlib.h>

// Global atomic variable
volatile unsigned irq_status = 0;

// Function to simulate interrupt handling
void simulate_interrupt_handling(void) {
    // Disable interrupts and save status
    irq_status = irq_disable();
    
    // Simulated critical section
    xtimer_usleep(1000000); // Sleep for 1 second
    
    // Restore interrupts
    irq_restore(irq_status);
}

// Main function
int main(void) {
    printf("Starting interrupt handling simulation on ESP32.\n");
    
    // Run interrupt handling simulation
    simulate_interrupt_handling();
    
    printf("Interrupt handling simulation completed.\n");
    
    return 0;
}