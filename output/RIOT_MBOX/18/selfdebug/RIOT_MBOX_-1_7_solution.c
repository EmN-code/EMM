#include <stdio.h>
#include "xtimer.h"
#include "msg.h"

#define MSG_QUEUE_SIZE 8
#define INTERVAL (1U * US_PER_SEC)

static msg_t message_queue[MSG_QUEUE_SIZE];

void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    unsigned cnt = 0;
    while (1) {
        xtimer_sleep(INTERVAL / US_PER_SEC);
        msg.content.value = cnt++;
        msg_send(&msg, msg_receive_pid(NULL));
    }
    return NULL;
}

int main(void) {
    msg_init_queue(message_queue, MSG_QUEUE_SIZE);

    char thread_stack[THREAD_STACKSIZE_MAIN];  // Assuming stack size

    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received a message with value: %u\n", (unsigned)msg.content.value);
    }

    return 0;
}