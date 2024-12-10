// Required header files in descending order
#include "ztimer.h"
#include "timex.h"
#include "stdio.h"
#include "periph/gpio.h"

#define INTERVAL (1U * 1000000U) // 1 second in microseconds

static void timer_callback(void *arg)
{
    (void)arg;
    static int led_state = 0;  // 0 = OFF, 1 = ON
    led_state = !led_state;
    if (led_state) {
        gpio_set(GPIO_PIN(PORT_A, 5));  // Turn LED ON (we assume the LED is connected to PA5)
    } else {
        gpio_clear(GPIO_PIN(PORT_A, 5));  // Turn LED OFF
    }
    printf("LED toggled, state: %d\n", led_state);
}

int main(void)
{
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    gpio_init(GPIO_PIN(PORT_A, 5), GPIO_OUT);

    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);

    while (1) {
        ztimer_sleep(ZTIMER_SEC);  // Sleep for a second in the main loop
    }

    return 0;
}
