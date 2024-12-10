
#include "ztimer.h"
#include "timex.h"
#include "xtimer.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"

#include "periph/gpio.h"  // Include the correct header for GPIO initialization

#define GPIO_PIN         GPIO_PIN(0, 18)  // Example GPIO pin

static volatile int atomic_var = 0;

static void gpio_isr_handler(void *arg)
{
    (void)arg;
    atomic_var++;  // Increment the atomic variable
}

int main(void)
{
    // Initialize the GPIO pin as input with interrupt
    gpio_init(GPIO_PIN, GPIO_IN);
    gpio_init_int(GPIO_PIN, GPIO_IN, GPIO_RISING, gpio_isr_handler, NULL);

    while (1) {
        // Main loop can be used for other tasks
        xtimer_sleep(1);  // Sleep for 1 second
    }

    return 0;
}
