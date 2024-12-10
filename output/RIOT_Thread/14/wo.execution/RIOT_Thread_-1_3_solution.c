#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 1)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

static msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %ld from thread %d\n", msg.content.value, msg.sender_pid);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    kernel_pid_t recv_pid = *((kernel_pid_t *)arg);
    msg_t msg;
    msg.type = 1;

    while (1) {
        msg.content.value = (uint32_t)(rand() % 100);
        msg_send(&msg, recv_pid);
        printf("Sent message: %ld\n", msg.content.value);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, STACKSIZE,
                                              THREAD_PRIORITY_RECEIVER, 0,
                                              receiver_thread, NULL, "receiver");

    printf("Receiver thread PID: %d\n", receiver_pid);

    thread_create(sender_stack, STACKSIZE,
                  THREAD_PRIORITY_MAIN, 0,
                  sender_thread, &receiver_pid, "sender");

    return 0;
}