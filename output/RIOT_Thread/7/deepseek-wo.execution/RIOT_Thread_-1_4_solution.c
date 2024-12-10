
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <timex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <time_units.h>

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

kernel_pid_t thread_create(char *stack, int stacksize, uint8_t priority, int flags, void (*task_func)(void *), void *arg, const char *name);
void msg_init_queue(msg_t *array, int num);
int msg_send_receive(msg_t *m, msg_t *reply, kernel_pid_t target_pid);
int msg_reply(msg_t *m, msg_t *reply);
int msg_receive(msg_t *m);

void sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;

    while (1) {
        msg_send_receive(&msg, &msg, receiver_pid);
        printf("Sender: Sent message with value %u\n", msg.content.value);
        sleep(1);
    }
}

void receiver_thread(void *arg) {
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %u\n", msg.content.value);

        msg_t reply;
        reply.type = 2;
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }
}

int main(void) {
    char stack1[STACK_SIZE];
    char stack2[STACK_SIZE];

    kernel_pid_t receiver_pid = thread_create(stack1, STACK_SIZE, 1, 0, receiver_thread, NULL, "receiver");
    kernel_pid_t sender_pid = thread_create(stack2, STACK_SIZE, 2, 0, sender_thread, &receiver_pid, "sender");

    (void)sender_pid; // To avoid unused variable warning

    return 0;
}
