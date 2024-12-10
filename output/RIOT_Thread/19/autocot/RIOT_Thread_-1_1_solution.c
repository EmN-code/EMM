#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)
#define QUEUE_SIZE (8)  // Number of messages
static msg_t queue[QUEUE_SIZE];
static char sender_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_stack[THREAD_STACKSIZE_DEFAULT];

void* sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 12345;
    while (1) {
        printf("Sender: Sending message\n");
        msg_send(&msg, thread_getpid_of(receiver_stack));  // Send msg to receiver
        xtimer_sleep(2);  // Wait for 2 seconds before sending again
    }
    return NULL;
}

void* receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(queue, QUEUE_SIZE);  // Initialize message queue
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: Received message with value %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), 10, 0,
                                             sender, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), 8, THREAD_CREATE_QUEUE,
                                             receiver, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    printf("Threads created\n");
    return 0;
}