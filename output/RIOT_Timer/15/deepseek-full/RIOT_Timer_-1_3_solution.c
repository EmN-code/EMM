
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <time_units.h>
#include <evtimer_msg.h>

#define LED_PIN 5
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_msg_t evtimer;
static mbox_t mbox;

void led_toggle(void) {
    // Dummy function to simulate LED toggle
    printf("LED toggled\n");
}

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Dummy value to indicate timer event
    msg_send(&msg, mbox_pid(&mbox));
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, &timer_callback, NULL, 1 * US_PER_SEC);

    msg_t msg;
    while (1) {
        // Wait for a message
        msg_receive(&msg);

        // Check if the message is from the timer
        if (msg.content.value == 1) {
            led_toggle();
        }
    }

    return 0;
}
