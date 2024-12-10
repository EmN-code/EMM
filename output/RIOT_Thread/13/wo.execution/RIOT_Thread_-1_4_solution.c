#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define SENDER_THREAD_PRIORITY  (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY  (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE        THREAD_STACKSIZE_MAIN
#define MSG_QUEUE_SIZE          (8)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];
static msg_t sender_queue[MSG_QUEUE_SIZE];
static msg_t receiver_queue[MSG_QUEUE_SIZE];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_function(void *arg) {
    (void) arg;
    msg_init_queue(sender_queue, MSG_QUEUE_SIZE);

    msg_t msg;
    msg.type = 1;

    while (1) {
        msg.content.value = (uint32_t) 0xAABBCCDD;
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }

    return NULL;
}

void *receiver_function(void *arg) {
    (void) arg;
    msg_init_queue(receiver_queue, MSG_QUEUE_SIZE);

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_THREAD_PRIORITY, 0,
                                 receiver_function, NULL, "receiver");

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_THREAD_PRIORITY, 0,
                               sender_function, NULL, "sender");

    return 0;
}
