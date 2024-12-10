#include <ztimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define SENDER_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIO (THREAD_PRIORITY_MAIN - 2)
#define MSG_QUEUE_SIZE (8)
#define SEND_INTERVAL (1000000) // time in microseconds (1 second)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_buffer[MSG_QUEUE_SIZE];
static msg_t msg;

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, SEND_INTERVAL / 1000);
        msg_send(&msg, receiver_pid);
        printf("Message sent!\n");
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t rcv_msg;

    while (1) {
        msg_receive(&rcv_msg);
        printf("Message received!\n");
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_buffer, MSG_QUEUE_SIZE);

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_THREAD_PRIO, 0, receiver_thread,
                                 NULL, "receiver");

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_THREAD_PRIO, 0, sender_thread,
                               NULL, "sender");

    printf("System initialized. Sender and receiver threads are running.\n");

    return 0;
}