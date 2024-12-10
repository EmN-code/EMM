#include <xtimer.h>
#include <msg.h>
#include <stdio.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define INTERVAL (1U)  // Send message every 1 second

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;
static msg_t msg_queue[8];
static msg_t msg;

void *sender_thread(void *arg) {
    (void)arg;
    while (1) {
        printf("Sending message...\n");
        msg.content.value = 42;  // Arbitrary message value
        msg_send(&msg, receiver_pid);
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t recv_msg;
    while (1) {
        msg_receive(&recv_msg);
        printf("Received message with value: %ld\n", recv_msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("--- Message Box Example on ESP32 ---");
    msg_init_queue(msg_queue, 8);

    sender_pid = thread_create(stack_sender, sizeof(stack_sender),
                               SENDER_PRIO, 0, sender_thread, NULL, "sender");
    receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver),
                                 RECEIVER_PRIO, 0, receiver_thread, NULL, "receiver");

    return 0;
}