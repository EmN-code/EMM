#include "ztimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "mutex.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

static msg_t sender_queue[8];
static msg_t receiver_queue[8];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    while (1) {
        msg_send(&msg, receiver_pid);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message of type: 0x%04x\n", msg.type);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(sender_queue, 8);
    msg_init_queue(receiver_queue, 8);

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                                 receiver_thread, NULL, "receiver");

    sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                               sender_thread, NULL, "sender");

    return 0;
}