#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_SENDER  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

static msg_t queue[8];

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    kernel_pid_t receiver_pid = *((kernel_pid_t *) arg);

    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        msg_send(&msg, receiver_pid);
        printf("Sent message: %d\n", i);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                             THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                             receiver, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER,
                  THREAD_CREATE_STACKTEST, sender, &receiver_pid, "sender");
    
    return 0;
}