#include "mma8x5x_params.h"
#include "net/emcute.h"
#include "stddef.h"
#include "event/thread.h"
#include "net/sock/udp.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "time_units.h"
#include "unistd.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "net/gcoap.h"
#include "xtimer.h"
#include "msg.h"
#include "stdio.h"
#include "ztimer.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "net/sock/util.h"
#include "evtimer_msg.h"

#define QUEUE_SIZE 4

static msg_t msg_queue[QUEUE_SIZE];
static char thread_stack[THREAD_STACKSIZE_DEFAULT];

void *msg_sender(void *arg) {
    (void)arg;
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    msg_t msg;
    msg.content.value = 42;

    while (1) {
        printf("Sending message...\n");
        mbox_send(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

void *msg_receiver(void *arg) {
    (void)arg;
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    msg_t msg;

    while (1) {
        mbox_receive(&mbox, &msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(
        thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, msg_sender, NULL, "msg_sender");

    kernel_pid_t receiver_pid = thread_create(
        thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, msg_receiver, NULL, "msg_receiver");

    printf("Message sender and receiver threads created:\nsender PID: %d\nreceiver PID: %d\n",
           sender_pid, receiver_pid);

    return 0;
}
