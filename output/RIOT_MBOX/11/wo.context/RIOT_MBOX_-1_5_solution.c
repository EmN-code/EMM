#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include <evtimer.h>
#include <msg.h>
#include <mbox.h>
#include <thread.h>

#define INTERVAL (2U * US_PER_SEC)
#define MBOX_SIZE 8

static kernel_pid_t receiver_pid;
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t mbox_queue[MBOX_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, mbox_queue, MBOX_SIZE);

    while (1) {
        msg_t msg;
        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Received message with value: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    printf("RIOT Message Box Example\n");
    
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), 
                                 THREAD_PRIORITY_MAIN - 1, 0, 
                                 receiver_thread, NULL, "receiver");

    evtimer_t evtimer;
    evtimer_msg_event_t event;
    evtimer_init_msg(&evtimer);
    
    evtimer_msg_event_t evt;
    evt.event.offset = INTERVAL;
    evtimer_add_msg(&evtimer, &evt, receiver_pid);

    while (1) {
        xtimer_sleep(INTERVAL / US_PER_SEC);
        msg_t msg = { .content.value = xtimer_usec_from_ticks(xtimer_now()) };
        mbox_send(&msg, &mbox);
    }
    return 0;
}