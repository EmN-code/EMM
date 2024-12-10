
#include "kernel_defines.h"
#include "mbox.h"
#include "thread.h"
#include "mutex.h"
#include "timex.h"
#include "xtimer.h"
#include "msg.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "time_units.h"
#include "esp_system.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define QUEUE_SIZE (16)
#define STACK_SIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1); // Send event every 1 second
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, STACK_SIZE, PRIORITY, 0, sender_thread, NULL, "sender");
    thread_create(NULL, STACK_SIZE, PRIORITY, 0, receiver_thread, NULL, "receiver");

    return 0;
}
