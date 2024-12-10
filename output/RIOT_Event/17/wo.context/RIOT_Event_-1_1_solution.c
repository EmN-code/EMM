#include <event/periodic.h>
#include <event/callback.h>

static void my_callback(void *arg) {
    printf("Callback function triggered\n");
}

static event_periodic_t periodic_event;

int main(void) {
    event_periodic_init(&periodic_event);
    event_periodic_set(&periodic_event, 1000000U, my_callback, NULL);
    event_periodic_start(&periodic_event);
    
    while(1) {
        // Sleep or wait for next interrupt
    }
    return 0;
}