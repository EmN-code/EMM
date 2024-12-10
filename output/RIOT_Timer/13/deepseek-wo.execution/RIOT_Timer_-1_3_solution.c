
#include "xtimer.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "mbox.h"
#include "timex.h"
#include "time_units.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

static void msg_handler(void *arg)
{
    (void)arg;
    // Toggle LED logic here
    // For example, if you have an LED connected to GPIO pin 5:
    // gpio_toggle(GPIO_PIN(0, 5));
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init(&evtimer, msg_handler);

    // Set up the event timer to trigger every second
    evtimer_add_msg(&evtimer, (xtimer_ticks32_t){TIMER_INTERVAL}, &mbox, NULL);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg, 1) == 1) {
            msg_handler(NULL);
        }
    }

    return 0;
}
