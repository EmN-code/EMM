#include <xtimer.h>
#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN + 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN + 2)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

static void *receiver_thread(void *arg) {
    msg_t msg;
    (void)arg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver got message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

static void *sender_thread(void *arg) {
    msg_t msg;
    (void)arg;
    msg.content.value = 42;
    kernel_pid_t receiver_pid = (kernel_pid_t)arg;
    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(3);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, THREAD_CREATE_STACKTEST,
                  sender_thread, (void *)receiver_pid, "sender");

    return 0;
}