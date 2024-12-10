#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "dht_params.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "msg.h"
#include "thread.h"
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include "ztimer.h"
#include "xtimer.h"

#define QUEUE_SIZE 8
static msg_t msg_queue[QUEUE_SIZE];

void *thread_handler(void *arg) {
    mbox_t mbox;
    msg_t msg;
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Received message\n");
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    msg_init_queue(msg_queue, QUEUE_SIZE);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_handler, NULL, "receiver_thread");

    while (1) {
        msg_t msg;
        msg_send(&msg, thread_getpid());
        printf("Message sent\n");
        xtimer_sleep(5);
    }
    return 0;
}