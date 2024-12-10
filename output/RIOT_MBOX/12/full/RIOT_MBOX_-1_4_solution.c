#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <event/thread.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <mmq8x5x_regs.h>
#include <mmq8x5x_params.h>
#include <mmq8x5x.h>
#include <MQTTClient.h>

#define PRIORITY        (THREAD_PRIORITY_MAIN - 1)
#define STACK_SIZE      (THREAD_STACKSIZE_MAIN)
#define MSG_QUEUE_SIZE  (8)

static char thread_stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

/* Event handler thread function */
void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg)) {
            printf("Message received from PID %d with value %d\n", msg.sender_pid, msg.content.value);
        }
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t event_pid = thread_create(thread_stack, sizeof(thread_stack), PRIORITY, 0,
                                           thread_handler, NULL, "event_handler");

    while (1) {
        xtimer_sleep(2);

        msg_t msg;
        msg.type = 0;
        msg.content.value = 42;

        if (event_pid != KERNEL_PID_UNDEF) {
            msg_send(&msg, event_pid);
            printf("Message sent with value %d\n", msg.content.value);
        }
    }

    return 0;
}