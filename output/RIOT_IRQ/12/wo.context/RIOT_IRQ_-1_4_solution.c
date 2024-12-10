#include <timer_arch.h>
#include <irq_arch.h>
#include <xtimer.h>

int counter = 0;  // Counter variable

void irq_handler(void *arg) {
    (void)arg;  // Prevents unused variable warning
    irq_disable();  // Disable interrupts
    counter++;  // Increment the counter
    irq_enable();  // Re-enable interrupts
}

int main(void) {
    // Set up a timer or other interrupt here
    // Example: pretend to set up a timer interrupt that triggers irq_handler
    
    while (1) {
        // Main loop does something
        // Optionally, print or check the value of the counter to verify interrupt handling
        xtimer_sleep(1);  // Wait for some time before re-checking
    }

    return 0;
}