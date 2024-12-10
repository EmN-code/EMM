#include <xtimer.h>
#include <irq_arch.h>
#include <stdio.h>
#include <stdatomic.h>

// Atomic variable
atomic_int counter = 0;

// Interrupt service routine simulation
void critical_function() {
    irq_disable();  // Disable interrupts

    // Critical section
    atomic_fetch_add(&counter, 1);
    printf("Counter: %d\n", counter);

    irq_restore();  // Restore interrupts
}

int main(void) {
    printf("---RIOT OS Interrupt Handling Example on ESP32---\n");

    // Simulate periodic interrupts
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second
        critical_function();  // Simulate interrupt
    }

    return 0;
}