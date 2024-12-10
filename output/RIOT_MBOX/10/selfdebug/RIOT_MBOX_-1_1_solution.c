#include <ztimer.h>
#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_MAIN];

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    puts("[Receiver] Thread started.");

    while (1) {
        msg_receive(&msg);
        printf("[Receiver] Message received: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_t msg;
    msg.content.value = 0;
    kernel_pid_t receiver_pid = thread_create(stack, sizeof(stack),
                                              THREAD_PRIORITY_MAIN - 1,
                                              THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL,
                                              "receiver");

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        msg.content.value++;
        printf("[Main] Sending message: %d\n", msg.content.value);
        msg_send(&msg, receiver_pid);
    }

    return 0;
}