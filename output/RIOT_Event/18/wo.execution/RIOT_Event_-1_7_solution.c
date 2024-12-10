#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mbox.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "event/timeout.h"
#include "event/callback.h"

void periodic_event_handler(void *arg) {
    printf("Event periodic callback triggered.\n");
    // Place your custom code here
}

int main(void) {
    printf("Initializing the timer event for ESP32\n");
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    kernel_pid_t target_pid = thread_getpid();

    evtimer_init_msg(&evtimer);  // Initialize the event timer for messages

    evtimer_add_msg(&evtimer, &event, target_pid);

    // Simulate a periodic timer setup
    while (1) {
        periodic_event_handler(NULL);
        xtimer_sleep(1);  // Triggers every 1 second
    }

    return 0;
}