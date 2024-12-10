#include <xtimer.h>
#include <msg.h>
#include <stdio.h>
#include <sched.h>

#define SENDER_THREAD_PRIORITY       (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY     (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE             THREAD_STACKSIZE_MAIN
#define NUM_MSGS                     16

msg_t msg_queue[NUM_MSGS];
char sender_stack[THREAD_STACKSIZE];
char receiver_stack[THREAD_STACKSIZE];

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, NUM_MSGS);

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with content: %d\n", msg.content.value);
        }
    }

    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = (int)random_bytes(NULL, sizeof(int));
        if (msg_send(&msg, receiver_pid) != 1) {
            printf("Failed to send message\n");
        } else {
            printf("Sent message with content: %d\n", msg.content.value);
        }
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                            sender, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                              receiver, NULL, "receiver");

    if (sender_pid < 0 || receiver_pid < 0) {
        printf("Failed to create threads\n");
        return -1;
    }

    printf("Threads created successfully\n");

    while (1) {
        // Keep main thread running
    }

    return 0;
}