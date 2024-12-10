#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include <evtimer.h>
#include <msg.h>

#define QUEUE_SIZE (8)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)

static msg_t queue[QUEUE_SIZE];
static evtimer_t evtimer;
static char stack[THREAD_STACKSIZE_DEFAULT];

void timer_callback(void *arg) {
    printf("Timer event triggered\n");
}

void *sender_thread(void *arg) {
    (void)arg;
    evtimer_mbox_event_t event;
    evtimer_add_mbox(&evtimer, &event, &queue);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received a message\n");
    }

    return NULL;
}

int main(void) {
    printf("RIOT Message Box Example\n");
    
    msg_init_queue(queue, QUEUE_SIZE);

    evtimer_init(&evtimer, timer_callback);
    evtimer_mbox_event_t event;
    evtimer_add_mbox(&evtimer, &event, &queue);
    
    thread_create(stack, sizeof(stack), SENDER_PRIO, 0, sender_thread, NULL, "sender");

    return 0;
}