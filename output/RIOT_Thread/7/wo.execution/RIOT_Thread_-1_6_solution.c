#include <ztimer.h>
#include <msg.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>

#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define RECEIVER_MSG_QUEUE_SIZE (8)

static char sender_thread_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT];

static kernel_pid_t receiver_pid;

static msg_t receiver_msg_queue[RECEIVER_MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 123; // Example message content

    while (1) {
        printf("Sender: Sending message...\n");
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(receiver_msg_queue, RECEIVER_MSG_QUEUE_SIZE);
    
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with content value: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), RECEIVER_THREAD_PRIORITY, 0, receiver_thread, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create receiver thread\n");
        return 1;
    }

    thread_create(sender_thread_stack, sizeof(sender_thread_stack), SENDER_THREAD_PRIORITY, 0, sender_thread, NULL, "sender");

    return 0;
}