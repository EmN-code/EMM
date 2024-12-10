
#include <stdatomic.h>
#include <periph/gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <periph/i2c.h>
#include <stddef.h>
#include <periph/rtc.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <xtimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>

// Define the GPIO pin to be used
#define GPIO_PIN GPIO_PIN(0, 0)

// Atomic variable to manage the interrupt state
atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

// Interrupt handler function
void gpio_isr(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    // Initialize the GPIO pin in input mode with pull-up resistor
    if (gpio_init(GPIO_PIN, GPIO_IN_PU) < 0) {
        puts("Error initializing GPIO pin");
        return 1;
    }

    // Initialize the GPIO interrupt
    if (gpio_init_int(GPIO_PIN, GPIO_IN_PU, GPIO_FALLING, gpio_isr, NULL) < 0) {
        puts("Error initializing GPIO interrupt");
        return 1;
    }

    // Main loop to check the interrupt flag
    while (1) {
        if (atomic_load(&interrupt_flag)) {
            puts("Interrupt occurred!");
            atomic_store(&interrupt_flag, 0);
        }
        // Add a small delay to avoid busy-waiting
        ztimer_sleep(ZTIMER_MSEC, 100);
    }

    return 0;
}
