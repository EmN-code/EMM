#include <stdio.h>
#include <stdlib.h>
#include "msg.h"
#include "evtimer.h"

#define THREAD_STACKSIZE_MAIN    2048
#define MSG_QUEUE_SIZE           8

static kernel_pid_t thread_pid;
static char thread_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;
static evtimer_msg_event_t evtimer_event;

void evtimer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    if (mbox_get(&mbox, &msg, 0)) {
        printf("Event received: %lu\n", msg.content.value);
    }
}

void *thread_handler(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init(&evtimer, THREAD_PRIORITY_MAIN - 1);
    evtimer_set_msg_event(&evtimer_event, &mbox, &thread_pid);
    evtimer_add_msg(&evtimer, &evtimer_event, EVTIMER_PERIODIC, 1000000);
    while (1) {
        evtimer_callback(NULL);
    }
    return NULL;
}

int main(void) {
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_handler, NULL, "msg_thread");
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }
    return 0;
}