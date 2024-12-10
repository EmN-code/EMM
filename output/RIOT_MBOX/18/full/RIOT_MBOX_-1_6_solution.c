#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <stddef.h>
#include <event/thread.h>
#include <net/sock/udp.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <evtimer.h>
#include <time_units.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <thread.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <msg.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <timex.h>
#include <mma8x5x_regs.h>
#include <net/sock/util.h>
#include <evtimer_msg.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];

static mbox_t mbox;
static msg_t msg_queue[8];

void *message_sender(void *arg) {
    (void)arg;
    int counter = 0;
    msg_t msg;

    while (1) {
        msg.content.value = counter++;
        mbox_post(&mbox, &msg, 0);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *message_receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_wait(&mbox, &msg);
        printf("Received message with value: %u\n", (unsigned)msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, msg_queue, 8);

    kernel_pid_t sender_pid = thread_create(stack, sizeof(stack),
                                            THREAD_PRIORITY_MAIN, 0,
                                            message_sender, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create sender thread\n");
        return 1;
    }

    kernel_pid_t receiver_pid = thread_create(stack, sizeof(stack),
                                              THREAD_PRIORITY_MAIN - 1, 0,
                                              message_receiver, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create receiver thread\n");
        return 1;
    }

    return 0;
}