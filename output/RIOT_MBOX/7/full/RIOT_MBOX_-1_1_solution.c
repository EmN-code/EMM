#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h>

#define INTERVAL (1000U * US_PER_MS) // 1-second interval
#define QUEUE_SIZE (8)

static msg_t queue[QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int count = 0;
    while (1) {
        xtimer_usleep(INTERVAL);
        msg.content.value = count++;
        msg_send(&msg, sched_active_thread->pid);
        printf("Sent message: %d\n", msg.content.value);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, QUEUE_SIZE);
    kernel_pid_t sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                                            THREAD_PRIORITY_MAIN - 1, 0,
                                            sender_thread, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                              THREAD_PRIORITY_MAIN - 1, 0,
                                              receiver_thread, NULL, "receiver");

    printf("Sender and Receiver threads started\n");

    return 0;
}
