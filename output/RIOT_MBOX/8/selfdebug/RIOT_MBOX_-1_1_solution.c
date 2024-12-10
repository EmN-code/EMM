#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define MSG_QUEUE_SIZE (8)
#define SLEEP_INTERVAL (2U) // Interval in seconds

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;  // Arbitrary message type
    while (1) {
        printf("Sending message\n");
        msg_send(&msg, thread_get(RECEIVER_PRIORITY));
        thread_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    printf("Receiver running\n");
    while (1) {
        msg_receive(&msg);
        printf("Message received: %d\n", msg.type);
    }
    return NULL;
}

int main(void) {
    printf("RIOT message box example\n");
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    kernel_pid_t receiver_pid = thread_create(msg_queue, sizeof(msg_queue),
                                              RECEIVER_PRIORITY, 0,
                                              receiver, NULL, "receiver");
    kernel_pid_t sender_pid = thread_create(msg_queue, sizeof(msg_queue),
                                            SENDER_PRIORITY, 0,
                                            sender, NULL, "sender");
    (void)receiver_pid;
    (void)sender_pid;
    while (1) {
        thread_sleep(SLEEP_INTERVAL);
    }
}