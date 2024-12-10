#include <xtimer.h>
#include <timer_arch.h>
#include <event.h>

static void _event_callback(void *arg);

int main(void) {
    evtimer_t timer;
    evtimer_event_t event;
    
    evtimer_init(&timer, _event_callback);
    
    uint32_t interval_ticks = SEC_IN_USEC;
    evtimer_set(&timer, interval_ticks);
    evtimer_callback_t callback = evtimer_periodic_init(_event_callback);
    evtimer_base_t base_time = TIMER_REGISTER(TIMER_ISR)

    base_time = evtimer_set(&timer, interval_ticks);
    
    while (1) {
        int init_result = evtimer_periodic_init_cb(&timer, &_event_callback, &arg);
        if (init_result != 0) {
            printf("Failed to initialize the periodic event callback\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Periodic event callback initialized\n");
            break;
        }
    }

    while (1) {
        int start_result = evtimer_start(&timer);
        if (start_result != 0) {
            printf("Failed to start the timer\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Timer started\n");
            break;
        }
    }

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}

static void _event_callback(void *arg) {
    puts("Event callback triggered");
}