#include <stdlib.h>
#include <stdio.h>
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2) // Assume your LED is connected to this GPIO

void led_on(void)
{
    gpio_set(LED_PIN);
}

void init_gpio(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

void timer_callback(void *arg)
{
    (void)arg;
    led_on();
}

int main(void)
{
    init_gpio();
    
    if (ztimer_periodic_start(ZTIMER_SEC, &timer_callback, NULL, 1) < 0) {
        printf("Error: unable to start timer\n");
        return 1;
    }

    while (1) {
        // Keep the main thread running
    }

    return 0;
}