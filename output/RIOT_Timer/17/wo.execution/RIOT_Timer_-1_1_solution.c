#include <timex.h>
#include <timer.h>
#include <stdio.h>
#include <thread.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) //1-second interval
#define LED_PIN        GPIO_PIN(0, 2) //Assuming the LED is connected to pin 2

static void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer triggered!\n");
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void)
{
    printf("Starting timer example\n");

    // Initialize the GPIO for the LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }
    
    // Initialize the timer
    if (timer_init(0, TIMER_INTERVAL, timer_callback, NULL)) {
        printf("Error initializing timer\n");
        return 1;
    }

    // Start the timer
    timer_start(0);

    // Keeping the main thread alive
    while (1) {
        thread_sleep();
    }

    return 0;
}