#include "thread.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 2)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 3)

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // example message
    msg_send(&msg, thread_getpid_of(&receiver_thread));
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            THREAD_PRIORITY_SENDER,
                                            THREAD_CREATE_STACKTEST,
                                            sender_thread, NULL, "sender_thread");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              THREAD_PRIORITY_RECEIVER,
                                              THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver_thread");

    (void)sender_pid;
    (void)receiver_pid;

    while (1) {
        thread_yield();
    }

    return 0;
}