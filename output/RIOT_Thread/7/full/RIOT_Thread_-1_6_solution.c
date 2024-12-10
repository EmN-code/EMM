#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <timex.h>
#include <time_units.h>
#include <stddef.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <arpa/inet.h>

#define SENDER_STACKSIZE (THREAD_STACKSIZE_MAIN / 2)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_MAIN / 2)
#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define QUEUE_SIZE (8)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

static msg_t msg_queue[QUEUE_SIZE];

void *sender(void *arg) {
    msg_t msg;
    int counter = 0;

    while (1) {
        msg.content.value = counter++;
        msg_send(&msg, thread_getpid_of(receiver_stack));

        printf("Sender: sent %d\n", msg.content.value);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *receiver(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: received %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIORITY, THREAD_CREATE_STACKTEST,
                  receiver, NULL, "receiver");

    return 0;
}