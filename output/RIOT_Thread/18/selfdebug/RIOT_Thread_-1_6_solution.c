#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

static msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example payload
    kernel_pid_t receiver_pid = *((kernel_pid_t *)arg);
    printf("Sending message to receiver PID: %d\n", receiver_pid);
    msg_send(&msg, receiver_pid);
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid;
    receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE, RECEIVER_PRIORITY, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    thread_create(sender_stack, SENDER_STACKSIZE, SENDER_PRIORITY, THREAD_CREATE_STACKTEST, sender_thread, &receiver_pid, "sender");
    return 0;
}