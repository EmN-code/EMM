
#include "xtimer.h"
#include "event/callback.h"
#include "thread.h"
#include "msg.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static kernel_pid_t event_thread_pid;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

static void event_thread(void *arg)
{
    (void)arg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer_init(&evtimer);
    event.callback = event_periodic_callback;
    event.arg = NULL;

    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
        evtimer_add(&evtimer, &event);
    }
}

int main(void)
{
    event_thread_pid = thread_create(stack, sizeof(stack), PRIORITY,
                                     THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    return 0;
}
