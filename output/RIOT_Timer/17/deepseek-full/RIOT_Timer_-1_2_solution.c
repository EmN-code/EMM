
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"

#define LED_PIN 5
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static evtimer_msg_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue));

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the periodic timer
    evtimer_add_msg(&evtimer, timex_set(TIMER_INTERVAL, 0), timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            timer_callback(msg.content.ptr);
            evtimer_add_msg(&evtimer, timex_set(TIMER_INTERVAL, 0), timer_callback, NULL);
        }
    }

    return 0;
}
