#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time_units.h>
#include <thread.h>
#include <msg.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <event/callback.h>
#include <MQTTClient.h>
#include <xtimer.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <evtimer_msg.h>

#define INTERVAL_SEC   (1U)    // 1 second

static void _callback(void *arg) {
    (void)arg;
    printf("Callback function executed.\n");
}

int main(void) {
    evtimer_t my_timer;
    evtimer_init_msg(&my_timer);

    evtimer_event_t event;
    evtimer_msg_event_callback_init(&_callback, NULL, &event);

    while (1) {
        evtimer_add_msg(&my_timer, &event, KERNEL_PID_DEFAULT);
        xtimer_sleep(INTERVAL_SEC);  // sleep for the defined interval
    }

    return 0;
}