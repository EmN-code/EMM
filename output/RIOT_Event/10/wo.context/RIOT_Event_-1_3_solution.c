#include <ztimer.h>
#include <event.h>
#include <evtimer.h>

#define TIMER_INTERVAL_SEC (1U)

static evtimer_msg_event_t periodic_event;

void on_periodic_event(void *arg)
{
    (void)arg;  // Silence unused parameter warning
    printf("Periodic event triggered!\n");
}

int main(void)
{
    printf("Starting periodic event example on RIOT with ESP32\n");

    /* Initialize event timer */
    evtimer_init_mbox(&periodic_event.event.timer);

    /* Set up periodic callback */
    evtimer_callback_t cb = { on_periodic_event, NULL };
    periodic_event.event.callback = cb;
    periodic_event.event.periodic = TIMER_INTERVAL_SEC * MS_PER_SEC;
    ztimer_periodic(&periodic_event.event.timer, cb);

    /* Initialize the event thread */
    char stack[THREAD_STACKSIZE_DEFAULT];
    event_queue_t queue;
    event_thread_init(&queue, stack, sizeof(stack), PRIO_DEFAULT);

    while (1) {
        evtimer_dispatch(&periodic_event.event.timer);
        thread_sleep(ztimer_now() + MS_PER_SEC * TIMER_INTERVAL_SEC);
    }

    return 0;
}
