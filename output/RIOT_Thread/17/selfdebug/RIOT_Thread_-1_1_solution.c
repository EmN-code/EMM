#include "msg.h"
#include "shell.h"
#include "thread.h"
#include "xtimer.h"

#define SENDER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define STACK_SIZE               (THREAD_STACKSIZE_MAIN)

char sender_stack[STACK_SIZE];
char receiver_stack[STACK_SIZE];

msg_t msg_queue[8];

kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    msg_init_queue(msg_queue, 8);
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) < 0) {
            continue;
        }
        printf("Received message with content: %d\n", msg.content.value);
    }

    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    xtimer_sleep(2);

    for (int i = 0; i < 5; i++) {
        msg.content.value = i;
        if (msg_send(&msg, receiver_pid) < 0) {
            printf("Error sending message %d\n", i);
        } else {
            printf("Sent message with content: %d\n", i);
        }
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, STACK_SIZE, RECEIVER_THREAD_PRIORITY,
                                 THREAD_CREATE_STACKTEST, receiver_thread,
                                 NULL, "receiver thread");
    
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread\n");
        return 1;
    }

    kernel_pid_t sender_pid = thread_create(sender_stack, STACK_SIZE, SENDER_THREAD_PRIORITY,
                                            THREAD_CREATE_STACKTEST, sender_thread,
                                            NULL, "sender thread");

    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating sender thread\n");
        return 1;
    }

    return 0;
}