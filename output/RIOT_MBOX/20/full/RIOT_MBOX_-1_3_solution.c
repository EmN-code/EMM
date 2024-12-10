#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "sched.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"

#define MSG_QUEUE_SIZE (8)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Message received. Sending reply.\n");
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;
    mbox_t mbox;
    evtimer_t evtimer;
    evtimer_mbox_event_t event;
    msg_t msg;

    // Create a new thread
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_handler, NULL, "thread_handler");

    // Initialize an event timer for the message box
    evtimer_init_mbox(&evtimer);
    evtimer_mbox_event_init(&event, &mbox);

    // Add a periodic event
    evtimer_add_periodic(&evtimer, &event, 2 * US_PER_SEC, &msg);

    while (1) {
        // Main thread could be doing other work
        xtimer_sleep(1);
    }

    return 0;
}