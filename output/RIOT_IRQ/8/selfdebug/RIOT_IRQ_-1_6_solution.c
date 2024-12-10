#include <thread.h>
#include <stdio.h>
#include <inttypes.h>
#include <irq.h>

// Declare atomic variable
static volatile uint32_t irq_count = 0;

// Function to simulate critical section
void simulate_critical_section(void) {
    printf("Entering critical section with interrupts disabled\n");
    irq_disable(); // Disable interrupts
    irq_count++;
    printf("Critical section executed. irq_count: %" PRIu32 "\n", irq_count);
    irq_enable(); // Enable interrupts
    printf("Exiting critical section with interrupts enabled\n");
}

int main(void) {
    // Initialize irq_count
    irq_count = 0;

    printf("Starting interrupt simulation\n");
    simulate_critical_section(); // Call function to handle critical section

    if (irq_count == 1) {
        printf("Interrupt handling verification successful!\n");
    } else {
        printf("Interrupt handling verification failed!\n");
    }

    return 0;
}
