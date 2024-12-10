
#include <thread.h>
#include <stdio.h>

typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_LEVELS - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 1)

char receiver_thread_stack[THREAD_STACKSIZE_MAIN];
char sender_thread_stack[THREAD_STACKSIZE_MAIN];

kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void) arg;  // To avoid unused parameter warning
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void) arg;  // To avoid unused parameter warning
    for (uint32_t i = 0; i < 10; i++) {
        msg_t msg;
        msg.type = 0;
        msg.content.value = i;
        msg.sender_pid = thread_getpid();
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                 THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                                 receiver_thread, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start receiver thread\n");
        return 1;
    }

    kernel_pid_t sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                                            THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                            sender_thread, NULL, "sender");

    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start sender thread\n");
        return 1;
    }

    return 0;
}