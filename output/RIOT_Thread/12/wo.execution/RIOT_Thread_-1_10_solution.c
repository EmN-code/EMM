#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "thread.h"
#include "msg.h"

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_DEFAULT (1024)
#define MSG_TYPE (0x1234)

static char sender_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_stack[THREAD_STACKSIZE_DEFAULT];

static kernel_pid_t receiver_pid;

void *sender(void *arg) {
    msg_t msg;
    msg.type = MSG_TYPE;
    msg.content.value = 42;
    
    while (1) {
        msg_send(&msg, receiver_pid);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

void *receiver(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                                 receiver, NULL, "receiver");
    
    thread_create(sender_stack, sizeof(sender_stack),
                  SENDER_PRIO, THREAD_CREATE_STACKTEST,
                  sender, NULL, "sender");

    return 0;
}