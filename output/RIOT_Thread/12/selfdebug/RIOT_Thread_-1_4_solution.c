#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static void *receiver_thread(void *arg) {
    msg_t msg;
    (void)arg;
    
    while (1) {
        msg_receive(&msg);
        printf("Receiver got message of type: %d\n", msg.type);
    }
    return NULL;
}

static void *sender_thread(void *arg) {
    msg_t msg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;

    while (1) {
        xtimer_sleep(2);
        msg.type = 1;  // example message type
        msg.content.value = 42;  // example content value
        msg_send(&msg, receiver_pid);
        printf("Sent message to receiver\n");
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                             RECEIVER_PRIO, 0,
                                             receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  SENDER_PRIO, 0,
                  sender_thread, &receiver_pid, "sender");

    return 0;
}