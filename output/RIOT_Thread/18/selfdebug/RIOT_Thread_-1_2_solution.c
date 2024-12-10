#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    msg_t msg;
    msg.content.value = *(int *)arg;
    printf("Sender: sending message with value %d\n", msg.content.value);
    msg_send(&msg, receiver_pid);
    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: received message with value %ld\n", (long int)msg.content.value);
    }
    return NULL;
}

int main(void) {
    int initial_value = 42;
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                                 receiver_thread, NULL, "receiver");
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST,
                               sender_thread, &initial_value, "sender");
    return 0;
}