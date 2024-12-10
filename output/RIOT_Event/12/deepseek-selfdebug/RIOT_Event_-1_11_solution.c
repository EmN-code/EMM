
#include "xtimer.h"
#include "event/periodic.h"
#include "thread.h"
#include "msg.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + sizeof(coap_pkt_t))
#define EVENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static event_queue_t event_queue;

static void periodic_event_cb(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);
    event_thread_init(&event_queue, event_thread_stack, EVENT_THREAD_STACKSIZE, EVENT_THREAD_PRIORITY);

    xtimer_t timer;
    timer.callback = periodic_event_cb;
    timer.arg = NULL;

    xtimer_set_periodic(&timer, XTIMER_TICKS(1000000)); // 1 second interval

    return 0;
}
