
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define SENDER_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIO (THREAD_PRIORITY_MAIN - 2)

static char stack_sender[THREAD_STACKSIZE_DEFAULT];
static char stack_receiver[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    char *message = "Hello from sender thread!";

    while (1) {
        msg.content.ptr = message;
        msg_send(&msg, thread_getpid(receiver_thread));
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(stack_receiver, sizeof(stack_receiver),
                  RECEIVER_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver");

    thread_create(stack_sender, sizeof(stack_sender),
                  SENDER_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    return 0;
}
