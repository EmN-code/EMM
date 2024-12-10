#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL (1U * US_PER_SEC)  // 1-second interval defined in microseconds

static void _timer_event_callback(void *arg)
{
    (void)arg; // Remove unused variable warning
    // Assume led_toggle is a custom function to toggle an LED
    led_toggle();
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    xtimer_t timer;

    // Initialize the event timer
    evtimer_init(&evtimer, _timer_event_callback);

    // Schedule the first event
    xtimer_set_msg(&timer, INTERVAL, _timer_event_callback, NULL);

    while (1) {
        // The loop below will ensure that the timer keeps being reset every second
        evtimer_add_msg(&evtimer, &event, thread_getpid());
        xtimer_sleep(1);
    }

    return 0;
}

void led_toggle(void)
{
    static int led_status = 0;

    if (led_status) {
        printf("LED OFF\n");
        led_status = 0;
        // Code to turn LED off
    }
    else {
        printf("LED ON\n");
        led_status = 1;
        // Code to turn LED on
    }
}