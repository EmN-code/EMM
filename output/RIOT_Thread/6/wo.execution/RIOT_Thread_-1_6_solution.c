#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define THREAD_STACKSIZE_MAIN  (1024)
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 2)

static char thread_stack_sender[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    for (uint32_t i = 0; i < 5; i++) {
        printf("[Sender] Sending message %lu\n", (unsigned long)i);
        msg.content.value = i;
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_create(thread_stack_sender, sizeof(thread_stack_sender), THREAD_PRIORITY_SENDER, 0, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("[Receiver] Received message: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}