#include "msg_types.h"
#include "msg.h"
#include "sys/random.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"

#define MESSAGE_QUEUE_SIZE (8)

static msg_t message_queue[MESSAGE_QUEUE_SIZE];
static kernel_pid_t pid;

void sender_thread(void) {
    msg_t msg;
    while (1) {
        msg.content.value = random_uint32();
        printf("Sending message with value: %ld\n", msg.content.value);
        msg_send(&msg, pid);
        xtimer_sleep(1);
    }
}

void receiver_thread(void) {
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);
    pid = thread_create(sender_thread, NULL);

    printf("Starting receiver thread\n");
    receiver_thread();

    return 0;
}