#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <time_units.h>
#include <MQTTClient.h>

static void _callback(void *arg) {
    (void)arg;
    // Your periodic task here
    printf("Callback executed.\n");
}

int main(void) {  
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    // Initialize event timer
    evtimer_init_msg(&evtimer);

    // Prepare periodic event
    evtimer_periodic_init(&event, &_callback, NULL, MS_PER_SEC);

    // Start the periodic event
    evtimer_add_periodic(&evtimer, &event);
    
    while (1) {
        // Main loop can include additional tasks
        ztimer_sleep(ZTIMER_MSEC, MS_PER_SEC);
    }

    return 0;
}
