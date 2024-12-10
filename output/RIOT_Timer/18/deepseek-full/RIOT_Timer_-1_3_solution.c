
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define LED_PIN 5
#define MSG_QUEUE_SIZE 8

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_msg_t evtimer;

void led_toggle(void) {
    // Simulate toggling the LED
    printf("LED toggled\n");
}

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.ptr = NULL;
    msg_send(&msg, thread_getpid());
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    evtimer_msg_event_t event;
    event.event.handler = timer_callback;
    event.event.arg = NULL;
    event.msg.sender_pid = thread_getpid();

    xtimer_t timer;
    timer.callback = (xtimer_callback_t)evtimer_add_msg;
    timer.arg = &event;
    timer.target = xtimer_now() + TIME_US(1000000);
    timer.long_target = timer.target.ticks64;

    // Start the timer
    xtimer_set(&timer, TIME_US(1000000));

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        led_toggle();
    }

    return 0;
}
