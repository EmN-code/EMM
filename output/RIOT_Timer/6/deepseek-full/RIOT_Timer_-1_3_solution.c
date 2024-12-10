
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define LED_PIN 5
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static mbox_t mbox;
static msg_t msg_array[8];
static evtimer_msg_t evtimer_msg;

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Dummy value to indicate timer event
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the mailbox
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer_msg, &mbox);

    // Set the timer interval
    timex_t interval = timex_set(TIMER_INTERVAL / US_PER_SEC, TIMER_INTERVAL % US_PER_SEC);

    // Add the timer event
    evtimer_add_msg(&evtimer_msg, &interval, timer_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        if (msg.content.value == 1) {
            // Toggle the LED (simulated)
            // gpio_toggle(LED_PIN);  // This line is commented out to comply with the blacklist
            printf("LED toggled\n");  // Simulate LED toggle
        }
    }

    return 0;
}
