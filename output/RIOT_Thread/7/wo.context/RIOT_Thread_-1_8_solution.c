#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE    THREAD_STACKSIZE_DEFAULT

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

static kernel_pid_t receiver_pid;

static void *receiver(void *arg) {
    msg_t msg;
    (void)arg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %ld\n", (long)msg.content.value);
    }

    return NULL;
}

static void *sender(void *arg) {
    msg_t msg;
    msg.content.value = 42;
    (void)arg;

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_PRIO, 0, receiver, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  SENDER_PRIO, 0, sender, NULL, "sender");

    return 0;
}
