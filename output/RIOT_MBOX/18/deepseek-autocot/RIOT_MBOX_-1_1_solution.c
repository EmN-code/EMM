
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE    10
#define SENDER_PRIO       5
#define RECEIVER_PRIO     4
#define INTERVAL_MS       1000000  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    int count = 0;

    while (1) {
        msg.content.value = count++;
        msg_send(&msg, receiver_pid);
        xtimer_usleep(INTERVAL_MS);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    receiver_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        RECEIVER_PRIO, 0,
        receiver_thread, NULL, "receiver");

    thread_create(
        thread_stack, sizeof(thread_stack),
        SENDER_PRIO, 0,
        sender_thread, NULL, "sender");

    return 0;
}
