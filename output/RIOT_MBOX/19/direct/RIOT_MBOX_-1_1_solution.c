#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL_SEC        (2)
#define NUM_MESSAGES        (5)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    for (int i = 0; i < NUM_MESSAGES; ++i) {
        msg.content.value = i;
        msg_send(&msg, thread_getpid());
        xtimer_sleep(INTERVAL_SEC);
    }
    return NULL;
}

void msg_handler(msg_t *msg) {
    printf("Received message with value: %d\n", msg->content.value);
}

int main(void) {
    puts("Message Box Example");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(stack,
                                            sizeof(stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            0,
                                            sender_thread,
                                            NULL,
                                            "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        msg_handler(&msg);
    }
    return 0;
}