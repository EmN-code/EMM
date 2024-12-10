/* Description: The program sets up a message box using a mailbox for handling messages at regular intervals and aims to address errors from implicit function declarations and strict prototypes in RIOT-OS development. */

// Importing required libraries
#include "ztimer.h"
#include "xtimer.h"
#include "event/thread.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "mbox.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Define the message queue size
#define QUEUE_SIZE (8)

// Create a message queue for communication
msg_t queue[QUEUE_SIZE];

static char stack[THREAD_STACKSIZE_MAIN];

// Callback handler for event timer
void evtimer_handler(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

// A thread function that waits for messages
void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        // Wait indefinitely for a message
        msg_receive(&msg);
        puts("Message received!");
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(queue, QUEUE_SIZE);

    // Create a thread for event handling
    kernel_pid_t event_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                                  THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    // Initialize a mailbox
    mbox_t mbox;
    msg_t mbox_queue[QUEUE_SIZE];
    mbox_init(&mbox, mbox_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_mbox_event_t evt;
    evtimer_init_mbox(&evt.event.timer);

    // Set the handler and add the event to the timer
    evt.event.handler = evtimer_handler;
    evtimer_add_mbox(&evt.event.timer, &evt, &mbox);

    // Main loop: simulate periodic events
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 5);
        msg_t m;
        m.type = 1;
        mbox_post(&mbox, &m, KERNEL_PID_UNDEF);
    }

    return 0;
}