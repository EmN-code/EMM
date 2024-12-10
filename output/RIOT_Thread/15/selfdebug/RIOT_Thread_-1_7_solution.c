#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN)
#define STACKSIZE (THREAD_STACKSIZE_MAIN)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

static msg_t msg_queue[8];
static kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void)arg; // unused
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Sender: Sent message with value %d\n", msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg; // unused
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT message communication example\n");

    msg_init_queue(msg_queue, 8);

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_PRIO, 0, receiver, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  SENDER_PRIO, 0, sender, NULL, "sender");

    return 0;
}