#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "irq.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

char thread_stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int priority = THREAD_PRIORITY_MAIN - 2;

    msg.type = 1;
    msg.content.value = 42;

    printf("Sending message with value: %lu \n", msg.content.value);
    while (1) {
        thread_create(thread_stack, sizeof(thread_stack), priority, 0, NULL, NULL, "receiver");
        msg_send_receive(&msg, &msg, thread_getpid());
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu \n", msg.content.value);
    }
}

int main(void) {
    printf("Starting threads...\n");
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    receiver_thread(NULL);
    return 0;
}