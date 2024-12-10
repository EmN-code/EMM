#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval
#define LED_PIN        GPIO_PIN(PORT_A, 0)  // Assuming the LED is connected to PORT_A pin 0

static void toggle_led(void)
{
    static int state = 0;  // keep track of LED state
    gpio_write(LED_PIN, state);
    state = !state;  // toggle state
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        toggle_led();
        xtimer_sleep(1);  // sleep for 1 second
    }

    return 0;
}