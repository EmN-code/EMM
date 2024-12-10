#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Arbitrary content for demonstration
    kernel_pid_t receiver_pid = thread_getpid_of(thread_get(RECEIVER_PRIORITY));

    if (msg_send(&msg, receiver_pid) <= 0) {
        printf("Sender: Message sending failed!\n");
    } else {
        printf("Sender: Message sent successfully!\n");
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: Message received with value: %d\n", msg.content.value);
        } else {
            printf("Receiver: Failed to receive message!\n");
        }
    }
    return NULL;
}

int main(void) {
    thread_t sender_thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_t receiver_thread_stack[THREAD_STACKSIZE_DEFAULT];

    /* Initialize the message queue used by the receiver */
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* Create the receiver thread */
    kernel_pid_t receiver = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                          RECEIVER_PRIORITY, THREAD_CREATE_STACKTEST,
                                          receiver_thread, NULL);

    if (receiver <= KERNEL_PID_UNDEF) {
        printf("Failed to create receiver thread!\n");
        return 1;
    }

    /* Create the sender thread */
    kernel_pid_t sender = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                                        SENDER_PRIORITY, THREAD_CREATE_STACKTEST,
                                        sender_thread, NULL);

    if (sender <= KERNEL_PID_UNDEF) {
        printf("Failed to create sender thread!\n");
        return 1;
    }

    return 0;
}
