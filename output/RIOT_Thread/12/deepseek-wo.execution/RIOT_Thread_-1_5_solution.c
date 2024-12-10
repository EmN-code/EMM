
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <timex.h>
#include <arpa/inet.h>
#include <mbox.h>
#include <event/thread.h>

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 8

typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

int msg_send_receive(msg_t *m, msg_t *reply, kernel_pid_t target_pid);
void msg_init_queue(msg_t *array, int num);
int msg_reply(msg_t *m, msg_t *reply);
int msg_receive(msg_t *m);

kernel_pid_t thread_create(char *stack,
                           int stacksize,
                           uint8_t priority,
                           int flags,
                           void (*task_func)(void *),
                           void *arg,
                           const char *name);

void sender_thread(void *arg) {
    msg_t msg, reply;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        msg_send_receive(&msg, &reply, (kernel_pid_t)arg);
        printf("Sender: Received reply with value %" PRIu32 "\n", reply.content.value);
        sleep(1);
    }
}

void receiver_thread(void *arg) {
    msg_t msg, reply;
    msg_init_queue((msg_t *)arg, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %" PRIu32 "\n", msg.content.value);
        reply.type = 2;
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }
}

int main(void) {
    msg_t msg_queue[MSG_QUEUE_SIZE];
    char stack1[STACK_SIZE], stack2[STACK_SIZE];

    kernel_pid_t receiver_pid = thread_create(stack1, STACK_SIZE, 1, 0, receiver_thread, msg_queue, "receiver");
    thread_create(stack2, STACK_SIZE, 2, 0, sender_thread, (void *)receiver_pid, "sender");

    return 0;
}
