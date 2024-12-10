
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;
static msg_t msg_queue[1];

void timer_callback(void *arg)
{
    (void)arg;
    // Toggle LED logic here
    // For example, if using a GPIO pin for LED:
    // gpio_toggle(LED0_PIN);
    // Note: gpio_toggle is not allowed, so simulate LED toggle here
    static int led_state = 0;
    led_state = !led_state;
    printf("LED state: %s\n", led_state ? "ON" : "OFF");
}

int main(void)
{
    msg_init_queue(msg_queue, 1);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, timex_set(TIMER_INTERVAL, 0), timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message, which will be the timer callback
        ((void (*)(void *))msg.content.ptr)(msg.content.ptr);
    }

    return 0;
}
