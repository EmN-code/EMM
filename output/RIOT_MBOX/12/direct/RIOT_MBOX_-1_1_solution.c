#include <stdio.h>
#include <stdlib.h>
#include "ztimer.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_INTERVAL     (1U * ZTIMER_SEC)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Send a message with a dummy value
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, SENDER_INTERVAL);
        printf("Sender: sending message with value %d\n", msg.content.value);
        if (msg_send(&msg, sched_active_pid) < 0) {
            puts("Sender: message could not be sent");
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    puts("Message Box Example Started");

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, msg_sender, NULL, "msg_sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: received message with value %d\n", msg.content.value);
    }

    return 0;
}