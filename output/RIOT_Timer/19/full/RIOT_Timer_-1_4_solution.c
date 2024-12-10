#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval
#define LED_PIN GPIO_PIN(PORT_A, 2)  // Assuming PORT_A is a valid port, update the pin as per your hardware

static void _toggle_led(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void)
{
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize GPIO\n");
        return 1;  // Return an error code
    }
    printf("Starting LED toggle timer example\n");

    xtimer_t timer;
    timer.callback = _toggle_led;
    timer.arg = NULL;

    xtimer_set(&timer, INTERVAL);

    while (1) {
        // Blocking wait for the timer to expire
        xtimer_sleep(INTERVAL);
        _toggle_led(NULL);
    }

    return 0;
}