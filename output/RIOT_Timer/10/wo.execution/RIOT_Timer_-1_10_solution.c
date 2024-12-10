#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(PORT_A, 4)

static xtimer_ticks32_t last_wakeup;

void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled!\n");
}

int main(void)
{
    printf("Starting the timer example on ESP32 using RIOT OS\n");

    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to init the GPIO pin\n");
        return 1;
    }

    last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}