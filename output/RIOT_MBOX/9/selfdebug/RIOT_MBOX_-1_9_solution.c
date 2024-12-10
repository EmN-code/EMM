#include <ztimer.h>
#include <stdio.h>
#include <thread.h>
#include <evtimer.h>
#include <msg.h>

// Stack size for the message handling thread
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)

// Event timer and message box structs
static evtimer_event_t event;
static msg_t msg_queue[4]; // Message queue with size power of two
static mbox_t mbox;

// Thread stack
static char thread_stack[THREAD_STACKSIZE_MAIN];

// Function to handle timer events
void *timer_event_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    mbox_init(&mbox, msg_queue, 4);

    // Initialize the event timer
    evtimer_init(&event, NULL);

    // Create a thread to handle events
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, timer_event_handler, NULL, "timer_handler");

    // Schedule an event every 5 seconds
    while (1) {
        event.msg.content.value = evtimer_now() + 5000000;
        evtimer_add(&event, &msg_send(mbox, &event.msg));
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}