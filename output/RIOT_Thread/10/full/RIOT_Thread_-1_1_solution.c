#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define RECONNECT_INTERVAL       (1U)

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;
char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Arbitrary message content

    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Sent message with content: %d\n", msg.content.value);
        xtimer_sleep(1); // Sleep for 1 second before sending again
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with content: %d\n", msg.content.value);
        }
        // Simulate network connection retry mechanism
        int rc = NetworkConnect();
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(RECONNECT_INTERVAL);
            continue;
        }
        break; // Exit loop if connected successfully
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER,
                                 THREAD_CREATE_STACKTEST,
                                 receiver_thread, NULL, "receiver");

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_SENDER,
                               THREAD_CREATE_STACKTEST,
                               sender_thread, NULL, "sender");

    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}