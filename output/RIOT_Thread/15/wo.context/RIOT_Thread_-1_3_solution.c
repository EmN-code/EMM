#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN + 1)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

msg_t msg_queue[4];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content
    kernel_pid_t receiver_pid = thread_getpid_of(&receiver_stack);
    if (receiver_pid == KERNEL_PID_UNDEF) {
        printf("Error: Could not find receiver PID.\n");
        return NULL;
    }
    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Message sent.\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 4);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER,
                                            THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender_thread");
    if (sender_pid == KERNEL_PID_UNDEF) {
        printf("Error: Could not create sender thread.\n");
        return 1;
    }

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER,
                                              THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver_thread");
    if (receiver_pid == KERNEL_PID_UNDEF) {
        printf("Error: Could not create receiver thread.\n");
        return 1;
    }

    return 0;
}