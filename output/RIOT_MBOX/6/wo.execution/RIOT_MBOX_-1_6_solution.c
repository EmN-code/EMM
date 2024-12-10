#include "msg.h"
#include "kernel_defines.h"
#include <stdio.h>
#include "xtimer.h"

#define MSG_INTERVAL (1U * US_PER_SEC) // 1 second

static kernel_pid_t thread_pid;
static msg_t message_queue[8];

void *event_thread(void *arg) {
    (void)arg; // Ignore the argument
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    thread_pid = thread_create(
        NULL, // Use internal stack
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_thread,
        NULL,
        "event_thread"
    );

    if (thread_pid <= KERNEL_PID_UNDEF) {
        puts("Thread creation failed.\n");
        return 1;
    }

    for (int i = 1; ; i++) {
        msg_t msg;
        msg.content.value = i;
        msg_try_send(&msg, thread_pid);
        xtimer_sleep(MSG_INTERVAL);
    }

    return 0;
}