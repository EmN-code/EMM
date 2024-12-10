#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "ztimer.h"
#include "MQTTClient.h"
#include "dht_params.h"

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char sender_thread_stack[SENDER_THREAD_STACKSIZE];
char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(&msg, 1);
    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Receiver: Received message from PID %d with type %u\n", msg.sender_pid, msg.type);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

static void *sender_thread(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = thread_getpid_from_name("receiver");
    if (receiver_pid == KERNEL_PID_UNDEF) {
        printf("Sender: Receiver not found\n");
        return NULL;
    }
    msg_t msg;
    msg.type = 1;
    msg.content.value = 12345;
    while (1) {
        msg.sender_pid = thread_getpid();
        if (msg_send(&msg, receiver_pid) == 1) {
            printf("Sender: Sent message to PID %d\n", receiver_pid);
        }
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, RECEIVER_THREAD_STACKSIZE, RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    if (receiver_pid == KERNEL_PID_UNDEF) {
        printf("Error: Unable to create receiver thread\n");
        return 1;
    }

    kernel_pid_t sender_pid = thread_create(sender_thread_stack, SENDER_THREAD_STACKSIZE, SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    if (sender_pid == KERNEL_PID_UNDEF) {
        printf("Error: Unable to create sender thread\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}