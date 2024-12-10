#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 0)
#define INTERVAL (1U * US_PER_SEC)

static void led_toggle(void)
{
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize LED_PIN as output
    if(gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Initialize ztimer
    ztimer_t timer = { .interval = INTERVAL, .callback = led_toggle, .arg = NULL };
    
    // Start timer that triggers every second
    while (1) {
        ztimer_set(ZTIMER_USEC, &timer, INTERVAL);  
        xtimer_sleep(1);
    }

    return 0;
}