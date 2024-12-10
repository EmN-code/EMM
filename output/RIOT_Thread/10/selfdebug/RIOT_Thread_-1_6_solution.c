#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)
#define THREAD_STACKSIZE_LARGE  (2 * THREAD_STACKSIZE_MAIN)

char stack_one[THREAD_STACKSIZE_DEFAULT];
char stack_two[THREAD_STACKSIZE_LARGE];

static kernel_pid_t sender_pid,
                    receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Sample message value
    xtimer_sleep(1);
    printf("Sender: Sending message...\n");
    if (msg_send(&msg, receiver_pid) < 0) {
        printf("Sender: Error sending message!\n");
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: Received message with value %ld\n", msg.content.value);
        }
    }   
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    sender_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN + 1,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    return 0;
}