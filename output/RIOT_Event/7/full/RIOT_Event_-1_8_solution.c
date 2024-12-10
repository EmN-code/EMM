#include <ztimer.h>
#include <net/sock/util.h>
#include <stdatomic.h>
#include <MQTTClient.h>
#include <net/gcoap.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <msg.h>
#include <dht_params.h>
#include <esp_system.h>
#include <timex.h>
#include <dht.h>
#include <stddef.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <esp_wifi.h>
#include <sys/socket.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <architecture.h>
#include <mutex.h>
#include <net/emcute.h>
#include <event/callback.h>
#include <stdlib.h>
#include <mma8x5x_params.h>
#include <thread.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <event/thread.h>

# define TIMER_INTERVAL (1000000U) /* 1 second */

void my_callback(void *arg) {
    printf("Callback function triggered!\n");
}

int main(void) {
    evtimer_t my_timer;
    evtimer_event_t my_event;

    /* Initialize event timer */
    evtimer_init(&my_timer, NULL);
    
    /* Configure the event */
    my_event.callback = my_callback;
    my_event.arg = NULL;

    /* Set the event in the timer to fire every second */
    evtimer_set(&my_timer, &my_event, TIMER_INTERVAL);

    /* Start event */
    evtimer_mbox_event_init(&my_timer); /* Use mailbox event init rather than direct start */

    /* Main loop */
    while(1) {
        evtimer_mbox_event_getnext(&my_timer); /* Use mailbox event to wait and get next event */
    }

    return 0;
}