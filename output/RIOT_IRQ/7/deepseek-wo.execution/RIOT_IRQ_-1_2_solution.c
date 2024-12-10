
#include "irq.h"
#include "thread.h"
#include "event/thread.h"

#define ESP32_IRQ_NUM 1

static volatile int atomic_var;

void irq_handler(void) {
    atomic_var = 1;
}

int main(void) {
    // Initialize the atomic variable
    atomic_var = 0;

    // Disable interrupts
    unsigned state = irq_disable();

    // Store the state of the atomic variable
    int initial_state = atomic_var;

    // Enable interrupts
    irq_restore(state);

    // Set the interrupt handler
    irq_set_exclusive_handler(ESP32_IRQ_NUM, irq_handler);

    // Enable the interrupt
    irq_enable(ESP32_IRQ_NUM);

    // Wait for the interrupt to occur
    thread_sleep();

    // Check if the interrupt handler modified the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt handling works correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
