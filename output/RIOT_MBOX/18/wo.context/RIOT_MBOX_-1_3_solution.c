#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "msg.h"
#include "thread.h"

#define INTERVAL (1U)
#define MAIN_QUEUE_SIZE (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *message_sender(void *arg) {
    msg_t msg;
    msg.content.value = 0;
    while (1) {
        xtimer_sleep(INTERVAL);
        msg.content.value++;
        msg_send(&msg, thread_getpid());
        printf("Message sent with value: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("Message Box Example");

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  message_sender, NULL, "message_sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}