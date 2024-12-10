
#include "kernel_defines.h"
#include "mutex.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "time_units.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

#define QUEUE_SIZE (16)
#define STACK_SIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(1000000); // Sleep for 1 second
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %u\n", (unsigned int)msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    char stack1[STACK_SIZE];
    char stack2[STACK_SIZE];

    thread_create(stack1, STACK_SIZE, PRIORITY, 0, sender_thread, NULL, "sender");
    thread_create(stack2, STACK_SIZE, PRIORITY, 0, receiver_thread, NULL, "receiver");

    return 0;
}
