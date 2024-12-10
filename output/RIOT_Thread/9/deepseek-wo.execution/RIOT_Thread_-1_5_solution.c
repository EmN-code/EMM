
#include <stdlib.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
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

#define STACKSIZE 2048
#define NUM_MESSAGES 16

static char stack[STACKSIZE];
static msg_t msg_queue[NUM_MESSAGES];

void sender_thread(void *arg) {
    msg_t msg;
    msg.content.value = 1234;
    msg.type = 1;

    while (1) {
        msg_send(&msg, (kernel_pid_t)arg, false);
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t receiver_pid = thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, sender_thread, (void *)receiver_pid, "sender");

    return 0;
}
