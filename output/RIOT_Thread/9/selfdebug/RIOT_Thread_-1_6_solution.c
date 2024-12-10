#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <kernel.h>
#include <msg.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE (8)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    for (uint32_t i = 1; i <= 5; ++i) {
        msg.content.value = i;
        printf("Sender: sending message %" PRIu32 "\n", msg.content.value);
        msg_send(&msg, thread_get(RECEIVER_PRIO));
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    while (1) {
        msg_receive(&msg);
        printf("Receiver: received message %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, THREAD_CREATE_STACKTEST, sender, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start sender thread\n");
        return -1;
    }

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0, receiver, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start receiver thread\n");
        return -1;
    }

    printf("Main: sender PID %" PRIkernel_pid_t ", receiver PID %" PRIkernel_pid_t "\n", sender_pid, receiver_pid);

    return 0;
}
