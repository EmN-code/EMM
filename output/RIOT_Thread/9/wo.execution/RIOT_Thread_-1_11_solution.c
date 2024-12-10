#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

char stack[THREAD_STACKSIZE_DEFAULT];

void sender_thread(void *arg) {
    (void)arg; // Unused parameter
    msg_t message;
    message.content.value = 42; // Example message content
    kernel_pid_t receiver_pid = *((kernel_pid_t *)arg);
    while (1) {
        // Send message to receiver thread
        msg_send(&message, receiver_pid);
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t received_msg;
    while (1) {
        // Await incoming message
        msg_receive(&received_msg);
        printf("Received value: %lu\n", received_msg.content.value);
    }
}

int main(void) {
    // Initialize message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    
    kernel_pid_t receiver_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                               CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        puts("Error: thread_create failed");
        return 1;
    }

    sender_thread((void *)&receiver_pid);

    return 0;
}