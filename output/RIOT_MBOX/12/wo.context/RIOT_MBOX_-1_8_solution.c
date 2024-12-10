#include <stdio.h>
#include <msg.h>
#include <kernel.h>
#include <xtimer.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define MAIN_QUEUE_SIZE (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(1);
        printf("Sender: Sending message with value %ld\n", msg.content.value);
        msg_send(&msg, KERNEL_PID_UNDEF);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %ld\n", msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            SENDER_PRIO, THREAD_CREATE_STACKTEST,
                                            sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    printf("Initialized sender with PID %d and receiver with PID %d\n",
           sender_pid, receiver_pid);

    return 0;
}