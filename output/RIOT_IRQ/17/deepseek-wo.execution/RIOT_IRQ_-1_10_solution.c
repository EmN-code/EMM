
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <xtimer.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Interrupt handler function
void interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Enable interrupts
    unsigned state = irq_enable();

    // Set the interrupt handler
    irq_set_exclusive_handler(ESP_INTR_NUM, interrupt_handler);

    // Enable the interrupt
    irq_enable_interrupt(ESP_INTR_NUM);

    // Restore the interrupt state
    irq_restore(state);

    // Sleep for a while to allow interrupts to occur
    xtimer_sleep(5);

    // Print the value of the atomic variable
    printf("Atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
