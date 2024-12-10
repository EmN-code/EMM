#include <ztimer.h>  // Timer management
#include <timex.h>   // Time extensions
#include <stdio.h>   // Standard input/output library
#include <msg.h>    // Message API
#include <thread.h>  // Thread support

#define MSG_TYPE_EVENT (0x01)
#define MAIN_QUEUE_SIZE (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static void *event_sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = MSG_TYPE_EVENT;

    while (1) {
        ztimer_sleep(ZTIMER_SEC, 5);  // Send a message every 5 seconds
        msg_send(&msg, thread_getpid());
        puts("Event sent!");
    }

    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    thread_create(NULL, 0, 0, THREAD_CREATE_STACKTEST, event_sender_thread, NULL, "event_sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_EVENT) {
            puts("Event received!");
        }
    }

    return 0;
}