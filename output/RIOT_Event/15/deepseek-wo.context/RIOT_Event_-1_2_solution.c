
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "event.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY THREAD_PRIORITY_MAIN - 1

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t event_thread_pid;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

static void *event_thread(void *arg)
{
    (void)arg;
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    xtimer_t timer;
    event_t event;

    event_init(&event, event_periodic_callback, NULL);

    while (1) {
        xtimer_set(&timer, 1000000); // 1 second
        event_post(&event_queue, &event);
        xtimer_usleep(1000000); // 1 second
    }

    return NULL;
}

int main(void)
{
    event_thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                                     THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                     event_thread, NULL, "event_thread");

    return 0;
}
