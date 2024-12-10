#include "ztimer.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "xtimer.h"

void evtimer_callback(void *arg) {
    (void)arg;  // Silence compiler warning for unused variable
    // Place your event handling code here
    printf("Periodic event triggered\n");
}

int main(void) {
    evtimer_msg_event_t event;
    kernel_pid_t pid = thread_getpid();
    evtimer_msg_t evtimer;
    evtimer_mbox_t mbox;
    char stack[THREAD_STACKSIZE_MAIN];
    
    msg_init_queue(mbox.msgs, MBOX_SIZE);
    evtimer_msg_init(&evtimer, &mbox, evtimer_callback, NULL);

    event.msg.type = 0;  // Define a message type for periodic events
    evtimer_add_msg(&evtimer, &event, pid);

    // 1-second timer
    evtimer_set_interval(&evtimer, 1 * US_PER_SEC);
    
    while (1) {
        // Waiting for the event to be triggered
        msg_receive(&event.msg);
    }

    return 0;
}