
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/timer.h"

#define MAIN_QUEUE_SIZE     (8)
#define TIMER_INTERVAL      (1000000U)  /* 1 second */

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static char thread_stack[THREAD_STACKSIZE_DEFAULT];

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  /* Example value */
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    /* Initialize the timer */
    timer_init(TIMER_DEV(0), 1000000, timer_callback, NULL);

    /* Create a thread to handle messages */
    kernel_pid_t pid = thread_create(thread_stack, sizeof(thread_stack),
                                     THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     (thread_task_func_t)msg_receive,
                                     NULL, "msg_handler");

    /* Start the timer */
    timer_set(TIMER_DEV(0), 0, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
