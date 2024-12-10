
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <xtimer.h>

// Define a global atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Interrupt handler function
void my_interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Disable interrupts and save the state
    unsigned state = irq_disable();

    // Set the exclusive interrupt handler
    irq_set_exclusive_handler(ESP32_IRQ_NUM, my_interrupt_handler);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    xtimer_sleep(5);

    // Restore the interrupt state
    irq_restore(state);

    // Print the value of the atomic variable
    printf("Atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
