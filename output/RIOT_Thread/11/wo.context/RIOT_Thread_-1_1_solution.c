#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE                (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE           (8)

char stack_sender[STACKSIZE];
char stack_receiver[STACKSIZE];

msg_t msg_queue[MSG_QUEUE_SIZE];

void *receiver_thread(void *arg) {
    msg_t msg;
    (void)arg;
    
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        if (msg_receive(&msg) < 0) {
            puts("Error receiving message");
            continue;
        }
        printf("Received message with content: %ld\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    msg_t msg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;

    while (1) {
        msg.content.value = 1234;  // Example content
        msg_send(&msg, receiver_pid);
        printf("Sent message\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver),
                                              THREAD_PRIORITY_RECEIVER, 0,
                                              receiver_thread, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create receiver thread");
        return 1;
    }

    thread_create(stack_sender, sizeof(stack_sender), THREAD_PRIORITY_SENDER, 0,
                  sender_thread, &receiver_pid, "sender");

    return 0;
}