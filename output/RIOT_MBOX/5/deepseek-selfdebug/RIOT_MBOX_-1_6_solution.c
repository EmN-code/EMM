
#include <stdio.h>
#include <ztimer.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <thread.h>

#define INTERVAL (1000000U)  // 1 second in microseconds

static void callback(void *arg)
{
    (void)arg;
    printf("Event triggered at count: %" PRIu32 "\n", event_periodic_callback_get_count(arg));
}

int main(void)
{
    event_queue_t queue;
    event_thread_init(&queue, NULL, 0, THREAD_PRIORITY_MAIN - 1);

    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, callback);
    event_periodic_callback_set_interval(&event, INTERVAL);
    event_periodic_callback_start(&event);

    while (1) {
        thread_yield();
    }

    return 0;
}
