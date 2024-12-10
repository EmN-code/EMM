
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE     (8)
#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO      (THREAD_PRIORITY_MAIN - 2)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t receiver_pid;

void *receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

void *sender(void *arg)
{
    (void)arg;
    msg_t msg;
    char *message = "Hello from sender!";

    while (1) {
        msg.type = 0;
        msg.content.ptr = message;
        msg_send(&msg, receiver_pid);
        xtimer_sleep(5); // Send a message every 5 seconds
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    receiver_pid = thread_create(
        thread_stack(receiver),
        sizeof(thread_stack(receiver)),
        RECEIVER_PRIO,
        THREAD_CREATE_STACKTEST,
        receiver,
        NULL,
        "receiver"
    );

    thread_create(
        thread_stack(sender),
        sizeof(thread_stack(sender)),
        SENDER_PRIO,
        THREAD_CREATE_STACKTEST,
        sender,
        NULL,
        "sender"
    );

    return 0;
}
