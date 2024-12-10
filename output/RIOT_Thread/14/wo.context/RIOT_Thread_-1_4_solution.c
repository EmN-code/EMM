#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN     (16)
#define THREAD_CREATE_WOUT_YIELD (4)
#define THREAD_CREATE_STACKTEST  (8)

char sender_thread_stack[THREAD_STACKSIZE_DEFAULT];
char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received value: %d from sender\n", msg.content.value);
    }

    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example value to be sent

    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Sent value: %d to receiver\n", msg.content.value);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create receiver thread\n");
        return -1;
    }

    sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_WOUT_YIELD | THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create sender thread\n");
        return -1;
    }

    return 0;
}