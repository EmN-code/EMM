
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"

#define LED_PIN             GPIO_PIN(0, 13)  // Example GPIO pin for LED
#define TIMER_INTERVAL      (1 * US_PER_SEC) // 1 second interval

static evtimer_msg_t evtimer;
static msg_t msg_queue[8];

void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void)
{
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, (evtimer_msg_event_t *)&timer_callback, NULL, kernel_pid_t target_pid);

    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            timer_callback(msg.content.ptr);
        }
    }

    return 0;
}
