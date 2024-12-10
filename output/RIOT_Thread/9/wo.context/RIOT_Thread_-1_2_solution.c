#include <thread.h>
#include <xtimer.h>
#include <stdio.h>

#define SENDER_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define RECEIVER_STACKSIZE THREAD_STACKSIZE_DEFAULT

#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

msg_t msg_queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // example message content
    printf("Sender: sending message with value %d\n", msg.content.value);
    msg_send(&msg, thread_getpid());
    xtimer_sleep(1); // ensure xtimer is included
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: received message with value %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));
    thread_create(receiver_stack, RECEIVER_STACKSIZE, RECEIVER_PRIORITY, THREAD_CREATE_SLEEPING, receiver, NULL, "receiver");
    thread_create(sender_stack, SENDER_STACKSIZE, SENDER_PRIORITY, THREAD_CREATE_SLEEPING, sender, NULL, "sender");
    return 0;
}