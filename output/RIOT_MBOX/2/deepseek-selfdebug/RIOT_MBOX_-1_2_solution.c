
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static msg_t msg_queue[8];

void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Event received!\n");
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, 8);

    kernel_pid_t pid = thread_create(
        thread_stack,
        sizeof(thread_stack),
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        event_thread,
        NULL,
        "event_thread"
    );

    if (pid <= 0) {
        printf("Failed to create event thread\n");
        return 1;
    }

    while (1) {
        msg_t msg;
        msg.content.value = 0;
        msg_send(&msg, pid);
        xtimer_sleep(5);
    }

    return 0;
}
