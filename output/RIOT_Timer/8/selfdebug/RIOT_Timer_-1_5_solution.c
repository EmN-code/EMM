#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>

#define INTERVAL_USEC (1U * US_PER_SEC)
#define LED_PIN (GPIO_PIN(0, 5)) // Assuming GPIO0 pin5 is where the LED is connected

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    static int state = 0;
    state = !state;
    gpio_write(LED_PIN, state);
    printf("LED is now %s\n", state ? "ON" : "OFF");
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    timer.callback = timer_callback;
    xtimer_set_periodic_msg(&timer, INTERVAL_USEC, NULL);
    return 0;
}