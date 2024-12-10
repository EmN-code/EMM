#include <event/periodic.h>
#include <event/callback.h>
#include <xtimer.h>
#include <stdio.h>

#define ONE_SEC_US (1000000U)

static event_periodic_t periodic_event;
static event_callback_t callback_event;
static kernel_pid_t main_pid;

static void callback(void *arg) {
    (void)arg; 
    printf("Event triggered!\n");
}

int main(void) {
    main_pid = thread_get_pid();
    
    event_callback_init(&callback_event, callback, NULL);
    event_periodic_init(&periodic_event, &callback_event.super, ONE_SEC_US);
    
    event_periodic_start(&periodic_event);
    
    while (1) {
        event_periodic_trigger(&periodic_event);
        xtimer_usleep(ONE_SEC_US);
    }
    return 0;
}