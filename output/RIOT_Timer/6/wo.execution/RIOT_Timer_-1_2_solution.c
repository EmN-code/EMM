#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * ZTIMER_MSEC)  // 1-second interval using ztimer
#define LED_PIN GPIO_PIN(PORT_A, 0)  // Assuming LED is connected to port A pin 0

static void led_toggle(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize ztimer
    ztimer_periodic(ztimer_now(), TIMER_INTERVAL, led_toggle, NULL);
    
    // Keep the main function running
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }
    return 0;
}