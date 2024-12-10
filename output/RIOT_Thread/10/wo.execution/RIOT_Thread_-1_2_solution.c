#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_MAIN)
#define SENDER_MSG_TYPE         (0x4545)

static char thread_one_stack[THREAD_STACKSIZE_MAIN];
static char thread_two_stack[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 10;
    msg.type = SENDER_MSG_TYPE;

    while(1) {
        puts("Thread One: Sending message to Thread Two.");
        msg_send(&msg, thread_two_pid);
        xtimer_sleep(2);
    }

    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;

    while(1) {
        if (msg_receive(&msg)) {
            if (msg.type == SENDER_MSG_TYPE) {
                printf("Thread Two: Received message with value: %lu\n", msg.content.value);
            }
        }
    }

    return NULL;
}

int main(void) {
    msg_t rcv_queue[8];
    msg_init_queue(rcv_queue, 8);

    thread_one_pid = thread_create(
        thread_one_stack, sizeof(thread_one_stack),
        THREAD_PRIORITY_MAIN + 1,
        THREAD_CREATE_STACKTEST,
        thread_one,
        NULL,
        "thread_one"
    );

    thread_two_pid = thread_create(
        thread_two_stack, sizeof(thread_two_stack),
        THREAD_PRIORITY_MAIN + 2,
        THREAD_CREATE_STACKTEST,
        thread_two,
        NULL,
        "thread_two"
    );

    return 0;
}
