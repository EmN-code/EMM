#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>
#include <event.h>
#include <event/callback.h>

#define QUEUE_SIZE (8)
#define INTERVAL (1000000U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;

void *thread_handler(void *arg) {
    (void)arg; // avoid unused parameter warning
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        puts("Message received!");
    }
    return NULL;
}

int main(void) {
    mbox_init(&mbox, queue, QUEUE_SIZE);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "receiver_thread");

    while (1) {
        msg_send(&queue, thread_handler);
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
    return 0;
}