#include <thread.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h>

#define THREAD_STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define MSG_QUEUE_SIZE (8)

char stack_msg_sender[THREAD_STACK_SIZE];
char stack_msg_receiver[THREAD_STACK_SIZE];

msg_t msg_queue_sender[MSG_QUEUE_SIZE];
msg_t msg_queue_receiver[MSG_QUEUE_SIZE];

static kernel_pid_t thread_msg_sender_pid;
static kernel_pid_t thread_msg_receiver_pid;

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;  // Example message type
    while (1) {
        msg_send(&msg, thread_msg_receiver_pid);
        printf("Message sent from sender to receiver.\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *msg_receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received by receiver.\n");
    }
    return NULL;
}

int main(void) {
    printf("Starting the RIOT messaging example\n");

    msg_init_queue(msg_queue_receiver, MSG_QUEUE_SIZE);

    thread_msg_receiver_pid = thread_create(stack_msg_receiver, sizeof(stack_msg_receiver),
                                            THREAD_PRIORITY_MAIN - 1, 0, msg_receiver, NULL, "msg_receiver");

    thread_msg_sender_pid = thread_create(stack_msg_sender, sizeof(stack_msg_sender),
                                          THREAD_PRIORITY_MAIN - 2, 0, msg_sender, NULL, "msg_sender");

    return 0;
}