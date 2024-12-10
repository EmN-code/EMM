
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/timer.h"

#define TIMER_DEV         (0)
#define TIMER_INTERVAL    (1000000U)  /* 1 second */
#define STACKSIZE         (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY          (THREAD_PRIORITY_MAIN - 1)

static msg_t msg_queue[8];
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

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    /* Create the receiver thread */
    receiver_pid = thread_create(
        thread_stack,
        STACKSIZE,
        PRIORITY,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    /* Initialize the timer */
    timer_init(TIMER_DEV, 1000000, NULL, NULL);

    /* Send messages at intervals */
    while (1) {
        xtimer_sleep(1);  /* Sleep for 1 second */
        msg_t msg;
        msg.content.value = 42;  /* Example message content */
        msg_send(&msg, receiver_pid);
    }

    return 0;
}
