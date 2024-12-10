#include "ztimer.h"
#include "xtimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include <stdio.h>
#include <stdlib.h>

// Assume an LED is connected to pin number 2 (you may need to adjust this per your setup)
#define LED_PIN 2
#define INTERVAL (1U * 1000U * 1000U) // 1 second in microseconds

static void _led_toggle(void *arg)
{
    (void)arg;
    // Implementation for toggling the LED
    // This is a placeholder for actual LED toggle code
    printf("Toggling LED on pin %d\n", LED_PIN);
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_msg_event_init(&evtimer);

    // Initialize timer
    evtimer_init_msg(&evtimer);

    // Set up the timer event
    evtimer_entry_t event;
    event.callback = _led_toggle;
    event.arg = NULL;
    event.offset = INTERVAL;

    evtimer_add_msg(&evtimer, &event);

    while (1) {
        xtimer_usleep(INTERVAL);  // Sleep for 1 second
        evtimer_run_msg(&evtimer);
    }

    return 0;
}