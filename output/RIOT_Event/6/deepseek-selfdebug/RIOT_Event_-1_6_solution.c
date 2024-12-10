
#include "xtimer.h"
#include "evtimer.h"
#include "thread.h"
#include "msg.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t thread_pid;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered\n");
}

static void event_thread(void *arg)
{
    (void)arg;
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init_msg(&evtimer);

    event.event.callback = periodic_callback;
    event.event.arg = NULL;
    event.msg.type = 0; // Set a valid message type

    while (1) {
        evtimer_add_msg(&evtimer, &event, thread_pid);
        xtimer_sleep(1);
    }
}

int main(void)
{
    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                               THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    return 0;
}
