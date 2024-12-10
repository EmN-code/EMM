
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <time_units.h>
#include <timex.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <MQTTClient.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <xtimer.h>
#include <dht_params.h>
#include <stddef.h>
#include <ztimer.h>

#define STACKSIZE 1024
#define NUM_MESSAGES 8

typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

msg_t msg_array[NUM_MESSAGES];

void sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 1234;

    while (1) {
        msg_send_receive(&msg, &msg, (kernel_pid_t)arg);
        printf("Sender: Sent message with value %lu\n", (unsigned long)msg.content.value);
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %lu\n", (unsigned long)msg.content.value);
        msg.content.value *= 2;
        msg_reply(&msg, &msg);
    }
}

int main(void) {
    msg_init_queue(msg_array, NUM_MESSAGES);

    kernel_pid_t sender_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, (void *)thread_getpid(), "sender");
    kernel_pid_t receiver_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}
