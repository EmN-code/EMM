#include <stdio.h>
#include <msg.h>
#include <thread.h>

#define SENDER_THREAD_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[SENDER_THREAD_STACKSIZE];
static char receiver_stack[RECEIVER_THREAD_STACKSIZE];

static msg_t msg_queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    kernel_pid_t receiver_pid = thread_getpid_of(NULL);
    printf("Sender: Sending message with value %d\n", msg.content.value);
    msg_send(&msg, receiver_pid);
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_tid = thread_create(receiver_stack, RECEIVER_THREAD_STACKSIZE,
                                              RECEIVER_THREAD_PRIORITY, 0, receiver, NULL, "receiver");
    thread_create(sender_stack, SENDER_THREAD_STACKSIZE, SENDER_THREAD_PRIORITY, 0, sender, NULL, "sender");
    return 0;
}