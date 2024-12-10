#include "thread.h"
#include "event/thread.h"
#include "event/callback.h"
#include "event/periodic.h"
#include "esp_partition.h"

#define PERIODIC_INTERVAL (1U * US_PER_SEC)

static event_callback_t periodic_callback;
static event_periodic_t periodic_event;

void my_callback(void *arg) {
    (void)arg;
    printf("Event callback triggered every second!\n");
}

int main(void) {
    event_callback_init(&periodic_callback, my_callback, NULL);
    event_periodic_init(&periodic_event, &periodic_callback, PERIODIC_INTERVAL);
    event_periodic_start(&periodic_event);
    
    while (1) {
        thread_yield();
    }
    return 0;
}