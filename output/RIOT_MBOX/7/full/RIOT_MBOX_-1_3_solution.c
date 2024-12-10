#include <xtimer.h>
#include <thread.h>
#include <timex.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <event/thread.h>

#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static evtimer_t _evtimer;
static char _stack[THREAD_STACKSIZE_MAIN];
static mbox_t _mbox;

void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    evtimer_msg_event_t event;

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize mbox
    mbox_init(&_mbox, _main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize evtimer
    evtimer_init(&_evtimer, &_mbox);

    // Set evtimer event
    event.super.mbox = &_mbox;
    event.period.tv_sec = 1;
    event.period.tv_usec = 0;
    evtimer_add(&_evtimer, &event.super, &(event.period));

    while (1) {
        // Wait for the message to be received within the specified period
        mbox_get(&_mbox, &msg);
        printf("Received an event message\n");
    }

    return NULL;
}

int main(void) {
    printf("Event timer and message box example for ESP32\n");

    // Create thread for event sending
    thread_create(_stack, sizeof(_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    return 0;
}