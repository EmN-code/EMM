
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "periph/timer.h"

#define TIMER_DEV         TIMER_DEV(0)
#define TIMER_INTERVAL    (1000000U)  // 1 second in microseconds
#define MSG_QUEUE_SIZE    8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t receiver_pid;

static void timer_cb(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Dummy value
    msg_send(&msg, receiver_pid);
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the receiver thread
    receiver_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread, NULL, "receiver");

    // Initialize the timer
    timer_init(TIMER_DEV, 1000000, timer_cb, NULL);

    // Set the timer to periodic mode
    timer_set_periodic(TIMER_DEV, 0, TIMER_INTERVAL);

    return 0;
}
