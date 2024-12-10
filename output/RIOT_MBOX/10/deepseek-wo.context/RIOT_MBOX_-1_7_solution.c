
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "event/callback.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define THREAD_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY     (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t sender_pid;

static void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received event message\n");
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    sender_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                               event_thread, NULL, "event_thread");

    xtimer_t timer;
    msg_t msg;

    xtimer_init(&timer);
    msg.type = 0;

    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
        msg_send(&msg, sender_pid);
    }

    return 0;
}
