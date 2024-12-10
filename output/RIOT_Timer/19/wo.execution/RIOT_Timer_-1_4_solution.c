```c
#include "xtimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "stdlib.h"
#include "stddef.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "mbox.h"
#include "stdatomic.h"
#include "unistd.h"
#include "stdbool.h"
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)  // Set timer interval to 1 second
#define LED_PIN  2                  // Example pin number for onboard LED

static void *timer_thread(void *arg) {
    (void)arg;
    msg_t msg;
    evtimer_mbox_t evtimer;

    msg_init_queue(msg, 8);

    evtimer_mbox_init(&evtimer);

    while (1) {
        msg_receive(&msg);

        if (evtimer_msg_received(&evtimer, &msg)) {
            // Toggle LED
            printf("Toggling LED\n");
            // Assuming GPIO is initialized elsewhere
            // gpio_toggle(LED_PIN);
        }
    }

    return NULL;
}

int main(void) {
    static evtimer_msg_t evtimer;
    static char timer_thread_stack[THREAD_STACKSIZE_MAIN];

    evtimer_msg_init(&evtimer);
    evtimer_msg_add_event(&evtimer, INTERVAL, 0);

    thread_create(timer_thread_stack, sizeof(timer_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_RUNNING,
                  timer_thread, NULL, "timer");

    while (1) {
        sleep(1);
    }

    return 0;
}
```
