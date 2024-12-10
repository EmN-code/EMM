#include <time_units.h>
#include <unistd.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <event/thread.h>
#include <stdio.h>

#define STACK_SIZE_THREAD 1024

char stack1[STACK_SIZE_THREAD];
char stack2[STACK_SIZE_THREAD];

static kernel_pid_t sender_id;
static kernel_pid_t receiver_id;

void *sender(void *arg) {
    (void)arg; // suppress unused parameter compiler warning
    msg_t msg;

    while (1) {
        msg.type = 0x1234; // example type, could use other value to indicate priority
        msg.content.value = rand();

        printf("Sender: Sending message with value %lu\n", msg.content.value);
        msg_send(&msg, receiver_id);

        sleep(1);
    }
}

void *receiver(void *arg) {
    (void)arg; // suppress unused parameter compiler warning
    msg_t msg;

    msg_init_queue((msg_t[4]){}, 4);

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message of type 0x%04x with value %lu from PID %d\n",
               msg.type, msg.content.value, msg.sender_pid);
    }
}

int main(void)
{
    receiver_id = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 
                                THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    sender_id = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 
                              THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    return 0;
}