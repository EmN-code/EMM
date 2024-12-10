#include "thread.h"
#include "msg.h"
#include "stdio.h"

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];

kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;

    while (1) {
        msg_send(&msg, receiver_pid);
        thread_yield();
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN - 1, 0,
                                 receiver, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_MAIN - 2, 0,
                  sender, NULL, "sender");

    return 0;
}