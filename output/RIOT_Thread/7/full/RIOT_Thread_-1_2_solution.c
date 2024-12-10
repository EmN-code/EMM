#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

#define THREAD_STACKSIZE_MAIN  (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN-1)
#define MESSAGE_QUEUE_SIZE    (8)

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received value: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            THREAD_PRIORITY_MAIN, 0,
                                            sender_thread, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              THREAD_PRIORITY_MAIN - 1, 0,
                                              receiver_thread, NULL, "receiver");

    printf("Sender thread PID: %d\n", sender_pid);
    printf("Receiver thread PID: %d\n", receiver_pid);

    return 0;
}