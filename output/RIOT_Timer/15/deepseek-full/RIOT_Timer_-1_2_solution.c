
#include <stdio.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <timex.h>
#include <ztimer.h>
#include <evtimer_msg.h>

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO pin 13 on port 0 for the LED

static evtimer_t evtimer;
static msg_t msg_queue[8];
static mbox_t mbox;

static void led_toggle(void) {
    // Simulate LED toggle logic here
    printf("LED toggled\n");
}

static void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Dummy value to indicate timer event
    mbox_put(&mbox, &msg);
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set up the timer event
    evtimer_mbox_event_t timer_event;
    timer_event.event.handler = timer_callback;
    timer_event.event.arg = NULL;
    timer_event.mbox = &mbox;

    // Add the timer event to the event timer
    evtimer_add_mbox(&evtimer, &timer_event, &mbox);

    // Set the timer to trigger every second
    ztimer_set(ZTIMER_MSEC, &timer_event.event.timer, 1000);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        if (msg.content.value == 1) {
            led_toggle();
        }
    }

    return 0;
}
