
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "periph/gpio.h"
#include "xtimer.h"
#include "thread.h"
#include "irq.h"

// Define the atomic variable
volatile uint32_t atomic_var = 0;

// Interrupt handler function
void gpio_isr(void *arg)
{
    (void)arg;
    // Disable interrupts to ensure atomic access
    unsigned state = irq_disable();
    atomic_var++;
    // Re-enable interrupts
    irq_restore(state);
}

int main(void)
{
    // Initialize the GPIO pin for interrupt
    gpio_t pin = GPIO_PIN(PORT_A, 0); // Example pin, adjust as needed
    gpio_init_int(pin, GPIO_IN, GPIO_RISING, gpio_isr, NULL);

    // Main loop
    while (1) {
        // Example usage of atomic variable
        printf("Atomic variable value: %lu\n", atomic_var);
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}
