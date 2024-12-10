
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define THREAD_STACKSIZE_SENDER (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_RECEIVER (THREAD_STACKSIZE_DEFAULT)

#define PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

#define MSG_QUEUE_SIZE (8)

static char stack_sender[THREAD_STACKSIZE_SENDER];
static char stack_receiver[THREAD_STACKSIZE_RECEIVER];

static kernel_pid_t pid_sender;
static kernel_pid_t pid_receiver;

static msg_t msg_queue_sender[MSG_QUEUE_SIZE];
static msg_t msg_queue_receiver[MSG_QUEUE_SIZE];

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue_receiver, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %" PRIu32 "\n", (uint32_t)msg.content.value);
    }

    return NULL;
}

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue_sender, MSG_QUEUE_SIZE);

    while (1) {
        xtimer_sleep(1);
        msg.type = 1;
        msg.content.value = (uint32_t)xtimer_now_usec();
        msg_send(&msg, pid_receiver);
    }

    return NULL;
}

int main(void)
{
    pid_sender = thread_create(stack_sender, sizeof(stack_sender), PRIORITY_SENDER,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    pid_receiver = thread_create(stack_receiver, sizeof(stack_receiver), PRIORITY_RECEIVER,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
