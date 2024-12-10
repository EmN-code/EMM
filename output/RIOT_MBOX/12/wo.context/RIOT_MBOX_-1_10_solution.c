#include "xtimer.h"
#include "msg.h"
#include "kernel.h"
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL            (1000000U) // 1 second in microseconds

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void *message_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content
    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, message_sender, NULL, "msg_sender");

    printf("Message receiver running\n");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}