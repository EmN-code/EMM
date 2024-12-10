
#include <stdio.h>
#include <periph/gpio.h>
#include <xtimer.h>
#include <msg.h>
#include <ztimer.h>
#include <timex.h>
#include <mbox.h>
#include <time_units.h>
#include <evtimer_msg.h>

#define LED_PIN GPIO_PIN(PORT_B, 5)
#define TIMER_INTERVAL (1 * US_PER_SEC)

static msg_t msg_queue[8];
static evtimer_msg_t evtimer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
