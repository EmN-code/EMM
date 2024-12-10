
#include "thread.h"
#include "event.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

static void event_periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_queue_t event_queue;
    event_t event;

    event_queue_init(&event_queue);
    event.handler = event_periodic_callback;

    while (1) {
        xtimer_sleep(1);
        event_post(&event_queue, &event);
    }

    return 0;
}
