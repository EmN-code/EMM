#include "ztimer.h"
#include "xtimer.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "msg.h"

#define THREAD_STACKSIZE (1024)
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

msg_t msg_queue[8];

void *receiver(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %ld\n", msg.content.value);
    }

    return NULL;
}

void *sender(void *arg) {
    (void) arg;
    msg_t msg;
    int counter = 0;
    kernel_pid_t receiver_pid = (kernel_pid_t)arg;
    while (1) {
        msg.content.value = counter++;
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    char sender_stack[THREAD_STACKSIZE];
    char receiver_stack[THREAD_STACKSIZE];

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                                              receiver, NULL, "receiver");

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST,
                                            sender, (void *)receiver_pid, "sender");

    (void)sender_pid;

    return 0;
}