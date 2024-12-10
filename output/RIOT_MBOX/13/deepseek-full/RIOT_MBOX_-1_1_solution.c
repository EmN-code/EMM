
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_DEFAULT];

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg.content.value = 1;  // Dummy event value
        msg_send(&msg, thread_getpid());
        xtimer_usleep(INTERVAL_MS);
    }

    return NULL;
}

int main(void)
{
    puts("Message Box Example");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received event with value: %d\n", msg.content.value);
    }

    return 0;
}
