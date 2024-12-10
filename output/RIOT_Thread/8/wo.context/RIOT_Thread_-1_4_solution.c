#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define SENDER_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_MAIN)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void)arg; // unused
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %" PRIu16 " and content value: %" PRIu32 "\n", msg.type, msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg; // unused
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Sent message\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create receiver thread\n");
        return 1;
    }

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 2, 0, sender_thread, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create sender thread\n");
        return 1;
    }

    return 0;
}
