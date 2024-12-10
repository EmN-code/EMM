#include <stdio.h>
#include "timer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // example pin, change according to your setup

static void timer_cb(void *arg) {
    (void)arg;
    static int led_status = 0;
    led_status = !led_status;
    gpio_write(LED_PIN, led_status);
    printf("LED is %s\n", led_status ? "ON" : "OFF");
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    
    tim_t timer_device = TIMER_DEV(0);
    if (timer_init(timer_device, 1, timer_cb, NULL) < 0) {
        printf("Error initializing timer\n");
        return 1;
    }

    timer_set_absolute(timer_device, timer_cb, NULL, 1);
    timer_start(timer_device);

    printf("Timer initialized and started\n");

    while (1) {
        // The main thread can execute other tasks
    }

    return 0;
}