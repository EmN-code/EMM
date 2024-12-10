#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "kernel.h"

#define MSG_TYPE_EVENT (0x01)

static msg_t msg_queue[8];
static kernel_pid_t event_sender_pid;

static void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = MSG_TYPE_EVENT;
    while (1) {
        xtimer_sleep(2);
        msg_send(&msg, event_sender_pid);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    event_sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_EVENT) {
            printf("Event received!\n");
        }
    }
    return 0;
}