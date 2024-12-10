#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <event/thread.h>
#include <evtimer_mbox.h>

// Define LED_PIN according to your board's specification
#define LED_PIN GPIO_PIN(PORT_A, 0) // Example for pin A0

void _evtimer_mbox_handler(evtimer_event_t *event) {
    (void)event;
    // Toggle LED state
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize GPIO as output for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Create an event timer
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    // Set up event for one-second intervals
    evtimer_event_t event;
    event.event.time = 1 * US_PER_SEC;

    // Add event to evtimer with the handler
    evtimer_add_msg(&evtimer, (evtimer_msg_event_t *)&event, thread_getpid());

    // Keep the program running to allow the event to trigger indefinitely
    while (1) {
        // Sleep to prevent busy-waiting
        xtimer_sleep(1);
    }

    return 0;
}