
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE         (8)
#define SENDER_PRIO            (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO          (THREAD_PRIORITY_MAIN - 2)
#define INTERVAL_MS            (1000000U)  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack_sender[THREAD_STACKSIZE_DEFAULT];
static char stack_receiver[THREAD_STACKSIZE_DEFAULT];

static void *receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

static void *sender(void *arg)
{
    (void)arg;
    msg_t msg;
    char *message = "Hello from sender!";

    msg.type = 0;
    msg.content.ptr = message;

    while (1) {
        xtimer_sleep(INTERVAL_MS / 1000000U);
        msg_send(&msg, (thread_t *)receiver, false);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(stack_receiver, sizeof(stack_receiver), RECEIVER_PRIO,
                  THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    thread_create(stack_sender, sizeof(stack_sender), SENDER_PRIO,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    return 0;
}
