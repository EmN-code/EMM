
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *timer_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_t reply;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg_send_receive(&msg, &reply, thread_getpid());
        printf("Received message: %" PRIu32 "\n", reply.content.value);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, timer_thread, NULL, "timer_thread");

    xtimer_t timer;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        msg_send(&msg, pid);
        xtimer_sleep(1);
    }

    return 0;
}
