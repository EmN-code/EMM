
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/sock/udp.h"

#define MSG_QUEUE_SIZE 8
#define STACK_SIZE 512

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
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

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1); // Blocking get
        printf("Received message with value: %u\n", (unsigned int)msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(NULL, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    return 0;
}
