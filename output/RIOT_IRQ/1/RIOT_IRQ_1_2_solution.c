
#include "ztimer.h"
#include "timex.h"
#include "xtimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "periph/gpio.h"

#define GPIO_PIN_NUM (0)  // Replace with the actual GPIO pin number you want to use

static volatile int atomic_var = 0;

static void gpio_isr(void *arg)
{
    (void)arg;
    atomic_var++;
}

int main(void)
{
    // Initialize the GPIO pin as input with interrupt
    gpio_init(GPIO_PIN(0, GPIO_PIN_NUM), GPIO_IN);
    gpio_init_int(GPIO_PIN(0, GPIO_PIN_NUM), GPIO_IN, GPIO_RISING, gpio_isr, NULL);

    while (1) {
        // Main loop can be used for other tasks
        xtimer_sleep(1);  // Sleep for 1 second
    }

    return 0;
}
