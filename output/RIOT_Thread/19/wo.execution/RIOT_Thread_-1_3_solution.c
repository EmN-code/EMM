#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"

#define SENDER_PRIO   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE_MAIN  (THREAD_STACKSIZE_MAIN)

char sender_stack[STACKSIZE_MAIN];
char receiver_stack[STACKSIZE_MAIN];

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) >= 0) {
            printf("Receiver got message with value: %lu and type: %u\n", msg.content.value, msg.type);
        }
    }
    return NULL;
}

void *sender(void *arg) {
    kernel_pid_t receiver_pid = *((kernel_pid_t *)arg);
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    printf("Sender sending message with value: %lu and type: %u\n", msg.content.value, msg.type);
    msg_send(&msg, receiver_pid);
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0, receiver, NULL, "receiver");
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender, &receiver_pid, "sender");

    thread_wakeup(receiver_pid);
    thread_wakeup(sender_pid);

    // Let threads run
    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return 0;
}