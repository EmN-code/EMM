#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE_DEFAULT 1024

#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW  (THREAD_PRIORITY_MAIN - 2)

char stack_low[THREAD_STACKSIZE_DEFAULT];
char stack_high[THREAD_STACKSIZE_DEFAULT];

static void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

static void *sender_low(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1;
    msg_send(&msg, thread_get(receiver_pid));
    return NULL;
}

static void *sender_high(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 2;
    msg_send(&msg, thread_get(receiver_pid));
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid;
    receiver_pid = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, THREAD_CREATE_STACKTEST, sender_low, NULL, "sender_low");
    thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, THREAD_CREATE_STACKTEST, sender_high, NULL, "sender_high");

    return 0;
}