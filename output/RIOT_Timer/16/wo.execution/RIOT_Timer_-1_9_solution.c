#define GPIO_PIN(port, pin) ((port) * 32 + (pin))
#include <ztimer.h>
#include <stdio.h>
#include <stdint.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Example LED_PIN, ensure this matches your hardware configuration

typedef struct {
    ztimer_t timer;
} app_t;

app_t app;

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
    ztimer_set(&app.timer, ZTIMER_MSEC, 1000);
}

int main(void)
{
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error: unable to initialize GPIO pin");
        return 1;
    }

    ztimer_init();
    ztimer_set(&app.timer, ZTIMER_MSEC, 1000);
    app.timer.callback = timer_callback;
    timer_callback(NULL);

    return 0;
}