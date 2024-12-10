
#include "msg.h"
#include "xtimer.h"
#include "irq.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdio.h"

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to handle the interrupt
void interrupt_handler(void)
{
    // Increment the atomic variable
    atomic_fetch_add(&atomic_var, 1);
    printf("Interrupt handled, atomic_var incremented to: %d\n", atomic_var);
}

int main(void)
{
    // Initialize the message queue
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(0, interrupt_handler);

    // Enable interrupts
    irq_restore(state);

    // Simulate an interrupt by calling the handler manually
    interrupt_handler();

    // Verify the atomic variable
    if (atomic_var == 1) {
        printf("Atomic variable verification successful: %d\n", atomic_var);
    } else {
        printf("Atomic variable verification failed: %d\n", atomic_var);
    }

    return 0;
}
