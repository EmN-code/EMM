
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>

#define MSG_QUEUE_SIZE  16
#define SEND_INTERVAL   (1 * 1000 * 1000U)  /* 1 second */

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg_send(&msg, thread_getpid());
        msg.content.value++;
        xtimer_usleep(SEND_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_t *sender_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender_thread"
    );

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
