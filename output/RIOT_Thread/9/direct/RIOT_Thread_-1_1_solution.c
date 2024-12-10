#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_PRIORITY_RECEIVER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER_LOW (THREAD_PRIORITY_MAIN - 2)
#define THREAD_PRIORITY_SENDER_HIGH (THREAD_PRIORITY_MAIN - 3)
#define MSG_QUEUE_SIZE 8

static kernel_pid_t receiver_pid;

static char receiver_stack[THREAD_STACKSIZE_DEFAULT];
static char sender_high_stack[THREAD_STACKSIZE_DEFAULT];
static char sender_low_stack[THREAD_STACKSIZE_DEFAULT];

msg_t msg_queue[MSG_QUEUE_SIZE];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        if (msg.content.value == 1) {
            printf("Received high priority message: %d\n", msg.content.value);
        } else if (msg.content.value == 2) {
            printf("Received low priority message: %d\n", msg.content.value);
        } else {
            printf("Received unknown message: %d\n", msg.content.value);
        }
    }
    return NULL;
}

void *sender_high_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1;
    while (1) {
        msg_send(&msg, receiver_pid);
        thread_yield_higher();  // Give chance to other threads
    }
    return NULL;
}

void *sender_low_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 2;
    while (1) {
        msg_send(&msg, receiver_pid);
        thread_yield_higher();  // Give chance to other threads
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                THREAD_PRIORITY_RECEIVER, 0,
                                receiver_thread, NULL, "receiver");

    thread_create(sender_high_stack, sizeof(sender_high_stack),
                  THREAD_PRIORITY_SENDER_HIGH, 0,
                  sender_high_thread, NULL, "sender_high");

    thread_create(sender_low_stack, sizeof(sender_low_stack),
                  THREAD_PRIORITY_SENDER_LOW, 0,
                  sender_low_thread, NULL, "sender_low");

    return 0;
}