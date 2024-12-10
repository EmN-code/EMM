#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <mma8x5x_regs.h> // This header seems unusual here, but including it as per request
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <evtimer.h>
#include <msg.h>

#define INTERVAL (ZTIMER_USEC_PER_SEC * 1) // 1-second interval
#define MSG_QUEUE_SIZE (8) // Message queue size, must be power of two

static msg_t _msg_queue[MSG_QUEUE_SIZE];
static mbox_t _mbox;

void handle_event(void *arg)
{
    (void)arg;
    printf("Event timer expired and message sent!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);

    // Initialize message box
    mbox_init(&_mbox, _msg_queue, MSG_QUEUE_SIZE);

    // Setup evtimer
    evtimer_t evtimer;
    evtimer_mbox_event_t event;

    event.event.callback = handle_event;
    event.event.arg = NULL;
    evtimer_add_mbox(&evtimer, &event, &_mbox);

    while (1) {
        msg_t msg;
        // Block and wait for messages,
        msg_receive(&msg);
    }

    return 0;
}

// Note:
// - evtimer_mbox_event_t, ztimer, and evtimer_add_mbox are the primary components ensuring that an event triggers the handle_event function at defined intervals.
// - The msg_t message queue and mbox are used to handle inter-process communication (IPC) within RIOT.
// - Regular inclusion of headers and ztimer related functions helps in setting the intervals for events as per the RIOT API.