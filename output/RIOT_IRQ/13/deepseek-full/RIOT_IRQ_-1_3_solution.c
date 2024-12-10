
#include <event/thread.h>
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <stdlib.h>
#include <stddef.h>

// Define a simple atomic variable structure
typedef struct {
    volatile int value;
} atomic_t;

#define ATOMIC_INIT(val) { (val) }

// Function to store a value atomically
void atomic_store(atomic_t *v, int x)
{
    v->value = x;
}

// Function to load a value atomically
int atomic_load(atomic_t *v)
{
    return v->value;
}

// Function to simulate an interrupt handler
void interrupt_handler(void)
{
    static atomic_t interrupt_handled = ATOMIC_INIT(0);
    atomic_store(&interrupt_handled, 1);
    printf("Interrupt handled!\n");
}

int main(void)
{
    // Initialize atomic variable
    atomic_t interrupt_handled = ATOMIC_INIT(0);

    // Enable interrupts
    unsigned state = irq_enable();

    // Simulate an interrupt
    interrupt_handler();

    // Restore interrupts
    irq_restore(state);

    // Check if the interrupt was handled
    if (atomic_load(&interrupt_handled)) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
