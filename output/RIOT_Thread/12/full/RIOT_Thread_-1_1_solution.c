#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 5
#define THREAD_PRIORITY_SENDER 8
#define THREAD_PRIORITY_RECEIVER 6
#define SENDER_MSG_COUNT 10

char thread_stack_sender[THREAD_STACKSIZE_DEFAULT];
char thread_stack_receiver[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int count = 0;

    while (count < SENDER_MSG_COUNT) {
        msg.content.value = count;
        printf("Sender: Sending message %d\n", count);
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
        count++;
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
    receiver_pid = thread_create(
        thread_stack_receiver, sizeof(thread_stack_receiver),
        THREAD_PRIORITY_RECEIVER, 0, receiver_thread, NULL, "receiver");

    sender_pid = thread_create(
        thread_stack_sender, sizeof(thread_stack_sender),
        THREAD_PRIORITY_SENDER, 0, sender_thread, NULL, "sender");

    return 0;
}