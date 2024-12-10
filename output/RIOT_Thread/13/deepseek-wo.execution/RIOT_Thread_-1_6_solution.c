
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time_units.h>
#include <stdatomic.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <dht.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define STACKSIZE 1024
#define NUM_MESSAGES 8

msg_t msg_queue[NUM_MESSAGES];

void sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 1234;

    while (1) {
        msg_send_receive(&msg, &msg, (kernel_pid_t)arg);
        printf("Sender: Sent message with value %" PRIu32 "\n", msg.content.value);
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %" PRIu32 "\n", msg.content.value);
        msg_reply(&msg, &msg);
    }
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t receiver_pid = thread_create(
        malloc(STACKSIZE),
        STACKSIZE,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    thread_create(
        malloc(STACKSIZE),
        STACKSIZE,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        (void *)receiver_pid,
        "sender"
    );

    return 0;
}
