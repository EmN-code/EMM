#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <thread.h>
#include <msg.h>

#define MSG_QUEUE_SIZE 8
#define INTERVAL 5

static msg_t msg_queue[MSG_QUEUE_SIZE];
kernel_pid_t thread_pid;
char thread_stack[THREAD_STACKSIZE_MAIN];

void *msg_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_pid = thread_create(
        thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, msg_handler, NULL, "msg_handler");

    uint32_t counter = 0;

    while (1) {
        msg_t msg;
        msg.content.value = counter++;
        msg_send(&msg, thread_pid);
        xtimer_sleep(INTERVAL);  // Simulate sending at intervals
    }

    return 0;
}
