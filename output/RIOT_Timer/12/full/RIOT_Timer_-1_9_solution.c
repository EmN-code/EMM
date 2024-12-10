#include "xtimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

#define LED_PIN 2 // Define the LED pin (GPIO2 is common for built-in LEDs on ESP32)
#define TIMER_INTERVAL (1LU * US_PER_SEC) // 1 second interval

void led_toggle(void)
{
    // Note: Assuming some gpio function to toggle the LED
    printf("LED toggled\n");
}

int main(void)
{
    printf("Starting LED blink example\n");

    evtimer_t timer;
    evtimer_event_t event;
    
    evtimer_init(&timer, NULL);

    event.callback = led_toggle;
    evtimer_add(&timer, &event, TIMER_INTERVAL);

    while (1) {
        evtimer_do_work(&timer);
        usleep(TIMER_INTERVAL);
    }

    return 0;
}