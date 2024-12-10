
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stddef.h>
#include <stdatomic.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <event/thread.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <thread.h>
#include <mbox.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <net/sock/util.h>
#include <time_units.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    int count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1); // Send event every 1 second
    }
}

int main(void)
{
    msg_t msg;
    char stack[STACK_SIZE];

    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(stack, STACK_SIZE, PRIORITY, 0, sender_thread, NULL, "sender");

    while (1) {
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }

    return 0;
}
