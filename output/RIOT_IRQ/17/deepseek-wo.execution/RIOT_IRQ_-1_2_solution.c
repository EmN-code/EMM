
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <event/thread.h>
#include <thread.h>

// Define an atomic variable type and functions
typedef volatile int atomic_t;
#define ATOMIC_INIT(val) (val)

static void atomic_store(atomic_t *var, int val) {
    *var = val;
}

static int atomic_load(atomic_t *var) {
    return *var;
}

// Define a simple interrupt handler
static void my_interrupt_handler(void) {
    printf("Interrupt handled!\n");
}

int main(void) {
    // Initialize an atomic variable
    atomic_t my_atomic = ATOMIC_INIT(0);

    // Store a value in the atomic variable
    atomic_store(&my_atomic, 1);

    // Disable interrupts
    unsigned state = irq_disable();

    // Check if interrupts are disabled
    if (atomic_load(&my_atomic) == 1) {
        printf("Interrupts are disabled, atomic value is 1\n");
    }

    // Restore interrupts
    irq_restore(state);

    // Check if interrupts are restored
    if (atomic_load(&my_atomic) == 1) {
        printf("Interrupts are restored, atomic value is still 1\n");
    }

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, my_interrupt_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Main loop
    while (1) {
        // Your main loop code here
    }

    return 0;
}
