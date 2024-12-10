#include "timex.h"
#include "timer.h"
#include "thread.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)
#define TIMER_INTERVAL 1

static void led_toggle(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    timer_t t;
    timer_init(&t, TIMER_FREQUENCY, led_toggle, NULL);
    timer_set(&t, TIMER_INTERVAL);
    thread_sleep();

    return 0;
}