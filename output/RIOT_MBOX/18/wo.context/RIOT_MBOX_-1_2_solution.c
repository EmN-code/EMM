#include <stdio.h>
#include <stdlib.h>
#include "msg.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define SENDER_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIORITY     (THREAD_PRIORITY_MAIN - 1)

static char sender_stack[SENDER_STACKSIZE];

void *_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // An example value to send
    while (1) {
        msg_send(&msg, sched_active_pid);
        printf("Message sent: value=%d\n", msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("RIOT Message Box Example");
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(sender_stack, sizeof(sender_stack),
                  SENDER_PRIORITY, THREAD_CREATE_STACKTEST,
                  _sender, NULL, "sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message: value=%d\n", (int)msg.content.value);
    }

    return 0;
}