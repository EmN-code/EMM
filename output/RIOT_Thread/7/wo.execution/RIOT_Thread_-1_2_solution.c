#include "thread.h"
#include "time_units.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include <stdio.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE     THREAD_STACKSIZE_MAIN

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

static msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    (void)arg;  // Prevent unused warning
    msg_init_queue(msg_queue, 8);

    msg_t msg;
    while (1) {
        msg_receive(&msg);  // Blocks until a message is received
        printf("Received message with type: %u\n", msg.type);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;  // Prevent unused warning
    msg_t msg;
    msg.type = 1;

    while (1) {
        msg.content.value = (uint32_t)esp_random();  // esp_random is just used for demonstration
        printf("Sending message with value: %lu\n", msg.content.value);
        msg_send(&msg, thread_get_id_from_name("receiver"));
        xtimer_usleep(1000000);
    }
    return NULL;
}

int main(void) {
    thread_create(receiver_stack, STACKSIZE, RECEIVER_PRIO, 0, receiver_thread, NULL, "receiver");
    thread_create(sender_stack, STACKSIZE, SENDER_PRIO, 0, sender_thread, NULL, "sender");

    return 0;
}