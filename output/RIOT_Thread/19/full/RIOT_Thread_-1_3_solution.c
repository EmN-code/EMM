#include <xtimer.h>
#include <stdlib.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_MAIN PRIORITY_MAIN
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[8];

void *sender_thread(void *arg) {
    msg_t msg;
    msg.content.value = 42; // Example message content

    while (1) {
        printf("Sending message...\n");
        msg_send(&msg, *((kernel_pid_t *)arg));
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with content: %d\n", msg.content.value);
        }
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    kernel_pid_t receiver_pid;
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver_thread");

    thread_create(sender_stack, sizeof(sender_stack), SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, sender_thread, &receiver_pid, "sender_thread");

    return 0;
}