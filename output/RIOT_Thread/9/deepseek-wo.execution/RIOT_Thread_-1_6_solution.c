
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <string.h>
#include <net/gcoap.h>

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

static char stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = (uint32_t)arg;

    while (1) {
        msg_send(&msg, (kernel_pid_t)arg);
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void) {
    kernel_pid_t sender_pid, receiver_pid;

    receiver_pid = thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");
    sender_pid = thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, sender_thread, (void *)receiver_pid, "sender");

    return 0;
}
