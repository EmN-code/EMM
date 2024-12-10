
#include <stdio.h>
#include "msg.h"
#include "thread.h"
#include "ztimer.h"

#define MSG_QUEUE_SIZE     (8)
#define STACK_SIZE         (1024)
#define PRIORITY           (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL           (1000000U)  /* 1 second in microseconds */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[STACK_SIZE];
static kernel_pid_t pid;

static void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received event at %" PRIu32 "\n", ztimer_now(ZTIMER_USEC));
    }

    return NULL;
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* Create the event thread */
    pid = thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    /* Initialize ztimer */
    ztimer_init();

    /* Set a periodic timer to send events */
    ztimer_set(ZTIMER_USEC, &pid, INTERVAL, (void (*)(void *))msg_send_to_self, NULL);

    return 0;
}
