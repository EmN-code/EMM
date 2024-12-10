#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define QUEUE_SIZE 8
#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)

static msg_t msg_queue[QUEUE_SIZE];
static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;
    while (1) {
        msg.content.value = counter++;
        if (msg_send(&msg, receiver_pid) < 0) {
            puts("Failed to send message");
        } else {
            printf("Message sent: %d\n", msg.content.value);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, QUEUE_SIZE);
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Message received: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    puts("Starting message box example...");

    receiver_pid = thread_create(stack, RECEIVER_THREAD_STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    sender_pid = thread_create(stack, SENDER_THREAD_STACKSIZE, THREAD_PRIORITY_MAIN - 2,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    while (1) {
        // Main thread can perform other tasks or sleep to save power
        xtimer_sleep(3600);
    }

    return 0;
}