
#include "msg.h"
#include "thread.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE (8)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define INTERVAL (1 * US_PER_SEC)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t receiver_pid;

void *receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    receiver_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        RECEIVER_PRIO,
        THREAD_CREATE_STACKTEST,
        receiver,
        NULL,
        "receiver"
    );

    msg_t msg;
    int counter = 0;

    while (1) {
        msg.content.value = counter++;
        msg_send(&msg, receiver_pid);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
