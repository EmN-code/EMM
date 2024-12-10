// This program demonstrates the use of a message box in RIOT OS on an ESP32.
// It periodically sends messages using an event timer. The timer and message box
// are set up, and the program runs indefinitely, sending an event every few seconds.

#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "event.h"
#include "msg.h"

static char event_thread_stack[THREAD_STACKSIZE_DEFAULT];
static event_queue_t event_queue;
static mbox_t mbox;
static msg_t msg_queue[8];

void *event_loop(void *arg) {
    (void)arg;
    while (1) {
        xtimer_sleep(5); // Sleep for 5 seconds
        msg_t msg;
        msg.type = 1; // Custom message type
        mbox_put(&mbox, &msg);
    }
    return NULL;
}

int main(void) {
    // Initialize message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    // Initialize event queue
    event_queue_init(&event_queue);

    // Create a thread for events
    thread_create(event_thread_stack, sizeof(event_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, event_loop, NULL, "event_thread");

    printf("Message box example running...\n");
    return 0;
}