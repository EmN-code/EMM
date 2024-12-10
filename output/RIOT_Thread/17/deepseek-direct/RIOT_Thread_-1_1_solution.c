
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

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    char *message = "Hello from sender thread!";

    while (1) {
        msg.content.ptr = (void *)message;
        msg_send(&msg, (kernel_pid_t)arg);
        xtimer_sleep(2); // Send a message every 2 seconds
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver),
                                              RECEIVER_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    thread_create(stack_sender, sizeof(stack_sender),
                  SENDER_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                  sender_thread, (void *)receiver_pid, "sender");

    return 0;
}
