
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define LED_PIN 5
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static mbox_t mbox;
static msg_t msg_queue[8];
static evtimer_msg_t evtimer;

static void _timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Dummy value to indicate timer event
    msg_send(&msg, mbox_pid);
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the periodic timer event
    evtimer_add_msg(&evtimer, &mbox, _timer_callback, NULL, TIMER_INTERVAL);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.content.value == 1) {
            // Toggle LED logic would go here if GPIO was allowed
            // For now, just print a message to indicate the timer event
            puts("Timer event triggered");
        }
    }

    return 0;
}
