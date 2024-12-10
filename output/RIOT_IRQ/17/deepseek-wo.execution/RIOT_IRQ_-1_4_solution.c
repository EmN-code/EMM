
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <xtimer.h>
#include <thread.h>
#include <unistd.h>
#include <irq.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Interrupt handler function
void irq_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
    printf("Interrupt handled, atomic_var incremented to: %d\n", atomic_var);
}

int main(void)
{
    // Save the current interrupt state
    unsigned int state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(ESP32_IRQ_NUM, irq_handler);

    // Enable the interrupt
    irq_enable_interrupt(ESP32_IRQ_NUM);

    // Restore the interrupt state
    irq_restore(state);

    // Trigger the interrupt manually to test the handler
    irq_trigger(ESP32_IRQ_NUM);

    // Wait for a while to see the output
    xtimer_sleep(1);

    // Disable the interrupt
    irq_disable_interrupt(ESP32_IRQ_NUM);

    // Print the final value of the atomic variable
    printf("Final value of atomic_var: %d\n", atomic_var);

    return 0;
}
