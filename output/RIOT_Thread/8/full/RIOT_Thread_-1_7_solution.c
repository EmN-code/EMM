#include "ztimer.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "msg.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "mbox.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "dht_params.h"
#include "timex.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "xtimer.h"
#include "kernel_defines.h"
#include "atomic.h"

#define MSG_QUEUE_SIZE (8U)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static msg_t queue[MSG_QUEUE_SIZE];

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 42;
        if (msg_send_receive(&msg, &msg, thread_getpid()) != 1) {
            printf("Sender: message send error\n");
        }
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: message received with value: %lu\n", msg.content.value);
        msg_reply(&msg, &msg);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, MSG_QUEUE_SIZE);

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY, 0,
                  sender, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY, 0,
                  receiver, NULL, "receiver");

    return 0;
}
