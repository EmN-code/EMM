#include "xtimer.h"
#include "msg.h"
#include "kernel_defines.h"
#include "kernel_types.h"
#include <stdio.h>

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOF_MSGS          (4U)

static char sender_stack[THREAD_STACKSIZE_DEFAULT];

static void *sender_handler(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        xtimer_sleep(2);
        msg.content.value = 42;
        printf("Sending value: %ld\n", msg.content.value);
        msg_send(&msg, SENDER_PRIO);
    }
    return NULL;
}

int main(void) {
    msg_t queue[NUMOF_MSGS];
    msg_init_queue(queue, NUMOF_MSGS);

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                  sender_handler, NULL, "sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }

    return 0;
}