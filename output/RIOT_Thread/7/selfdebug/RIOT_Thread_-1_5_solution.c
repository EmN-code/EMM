#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 1

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

void *sender(void *arg) {
    uint32_t value = 1;
    msg_t msg;
    msg.content.value = value;
    printf("[Sender] Sending message with value: %lu\n", (unsigned long)msg.content.value);
    msg_send(&msg, thread_getpid());  // Assume msg_send needs to be handled for receiving process
    return NULL;
}

void *receiver(void *arg) {
    msg_t msg;
    msg_receive(&msg);
    printf("[Receiver] Received message with value: %lu\n", (unsigned long)msg.content.value);
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(
        sender_stack, sizeof(sender_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(
        receiver_stack, sizeof(receiver_stack),
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        receiver, NULL, "receiver");

    printf("Threads created: sender=%" PRIkernel_pid_t ", receiver=%" PRIkernel_pid_t "\n", sender_pid, receiver_pid);

    return 0;
}