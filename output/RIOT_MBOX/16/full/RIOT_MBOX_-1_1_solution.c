#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <evtimer.h>

#define TIMER_INTERVAL_USEC   (1000000U)  // 1 second in microseconds
#define MAX_MSGS              (8)

static msg_t msg_queue[MAX_MSGS];
static evtimer_t evtimer;
static evtimer_msg_event_t evtimer_event;

static void timer_callback(evtimer_event_t *event)
{
    (void)event;  // Cast to void to silence unused parameter warning
    printf("Event timer triggered! Sending message.\n");
    
    // Create a message
    msg_t msg;
    msg.content.value = 42;  // Arbitrary message content

    // Send the message
    msg_send(&msg, msg_receive_pid);  // assuming msg_receive_pid is the process ID of the receiver
}

int main(void)
{
    msg_init_queue(msg_queue, MAX_MSGS);

    evtimer_init(&evtimer, EVTIMER_MSG);

    evtimer_event.callback = timer_callback;
    evtimer_event.msg.type = EVTIMER_MSG;
    evtimer_event.msg.content.ptr = &evtimer_event;

    // Initialize the event timer
    evtimer_add(&evtimer, &evtimer_event.event, TIMER_INTERVAL_USEC);
    
    printf("Initialized event timer. It will trigger every second.\n");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}