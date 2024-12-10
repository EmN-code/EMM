
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <MQTTClient.h>
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
#include <mma8x5x_regs.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <net/sock/util.h>
#include <time_units.h>
#include <kernel_defines.h>

#define QUEUE_SIZE (16)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    char stack[STACK_SIZE];
    thread_create(stack, STACK_SIZE, PRIORITY, 0, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}
