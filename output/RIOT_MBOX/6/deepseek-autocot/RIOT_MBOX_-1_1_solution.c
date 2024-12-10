
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

void send_event(void)
{
    static int count = 0;
    char msg_content[32];
    msg_t msg;

    snprintf(msg_content, sizeof(msg_content), "Event %d", count++);
    msg.content.ptr = msg_content;
    msg.type = 0;

    msg_send(&msg, receiver_pid);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    receiver_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver");

    while (1) {
        send_event();
        xtimer_sleep(INTERVAL_MS / MS_IN_USEC);
    }

    return 0;
}
