#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <thread.h>
#include <event/thread.h>
#include <event/callback.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define INTERVAL (1U * US_PER_SEC)

static evtimer_t event_timer;

void event_callback(void *arg) {
    (void)arg;
    printf("Event triggered every second!\n");
}

int main(void) {
    evtimer_init_msg(&event_timer);

    evtimer_event_t evt = {
        .callback = event_callback,
    };
    evtimer_add_msg(&event_timer, &evt, INTERVAL);

    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    evtimer_run(&event_timer);

    return 0;
}
