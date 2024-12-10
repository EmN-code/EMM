#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define STACK_SIZE               (THREAD_STACKSIZE_DEFAULT)

char sender_stack[STACK_SIZE];
char receiver_stack[STACK_SIZE];

msg_t msg_queue[8];

static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1; // set message type
    msg.content.value = 42; // set message content

    printf("Sender: sending message with value: %u\n", msg.content.value);
    msg_send(&msg, receiver_pid);

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);

    if (msg_receive(&msg) == 1) {
        printf("Receiver: received message with value: %u\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, 0,
                  sender_thread, NULL, "sender");

    return 0;
}