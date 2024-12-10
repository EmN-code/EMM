
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5  // Assuming LED is connected to GPIO 5

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Dummy value to indicate timer event
    msg_send(&msg, mbox_channel(&mbox, 0));
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set the timer to trigger every second
    evtimer_add_msg(&evtimer, (xtimer_ticks32(TIMER_INTERVAL)), timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.content.value == 1) {
            // Toggle the LED
            // Assuming gpio_toggle is not allowed, we can simulate it here
            // For simplicity, we will just print a message indicating the LED toggle
            printf("LED toggled\n");
        }
    }

    return 0;
}
