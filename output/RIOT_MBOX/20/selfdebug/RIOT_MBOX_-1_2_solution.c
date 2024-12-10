#include <ztimer.h>
#include <stdio.h>
#include <thread.h>
#include <sys/msg.h>
#include <evtimer.h>
#include <event.h>

#define MAIN_QUEUE_SIZE  (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void event_loop(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    event.event.ticks32 = ztimer_now(ZTIMER_MSEC) + 1000;

    // Initialize mailbox
    mbox_t mbox;
    msg_t mbox_queue[8];
    mbox_init(&mbox, mbox_queue, 8);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        msg_t msg;
        // Wait for an incoming message
        msg_receive(&msg);
        printf("Event fired!\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("RIOT mbox example");

    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");
    return 0;
}