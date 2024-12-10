#include <xtimer.h>
#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)
#define TIMER_INTERVAL_SEC (1U)

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);

    ztimer_ticks_t timer_interval = ZTIMER_MSEC(TIMER_INTERVAL_SEC * 1000);
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    ztimer_set(ZTIMER_MSEC, &timer, timer_interval);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, timer_interval);
    }
    return 0;
}