#include <stdio.h>
#include <msg.h>
#include <evtimer.h>
#include <ztimer.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define INTERVAL (1U * 1000000U)  // Interval of 1 second
#define MESSAGE_QUEUE_SIZE (8)

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];
static evtimer_t evtimer;
static evtimer_msg_event_t evtimer_event;

static void _event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_send(&msg, thread_getpid());
    printf("Event message sent!\n");
}

int main(void)
{
    printf("RIOT OS Message Box Example on ESP32\n");

    // Initialize the message queue
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);

    // Set up the event timer
    evtimer_init(&evtimer, ZTIMER_USEC, _event_callback, NULL);
    evtimer_add_msg(&evtimer, &evtimer_event.event, INTERVAL, thread_getpid(), NULL);

    // Event loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);  // Wait for message
        printf("Received event message\n");
    }

    return 0;
}