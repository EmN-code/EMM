
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE    10
#define INTERVAL_MS       1000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 1; // Example event value
        msg_send(&msg, (kernel_pid_t)arg);
        xtimer_sleep(INTERVAL_MS / 1000);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received event with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(
        thread_stack(sender_thread),
        sizeof(thread_stack(sender_thread)),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender");

    kernel_pid_t receiver_pid = thread_create(
        thread_stack(receiver_thread),
        sizeof(thread_stack(receiver_thread)),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver");

    sender_thread((void *)receiver_pid);

    return 0;
}
