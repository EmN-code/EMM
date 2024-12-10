
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>

#define MSG_QUEUE_SIZE    10
#define INTERVAL_MS       1000000  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg_send(&msg, thread_getpid());
        msg.content.value++;
        xtimer_usleep(INTERVAL_MS);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_t *sender = thread_create(
        thread_stack(NULL, 512),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
