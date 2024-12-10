#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // example message content
    while (1) {
        xtimer_sleep(5);  // send message every 5 seconds
        if (msg_send(&msg, thread_getpid()) <= 0) {
            printf("Failed to send message\n");
        }
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");
    return 0;
}