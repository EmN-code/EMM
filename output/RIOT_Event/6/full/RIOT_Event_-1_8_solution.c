#include <net/sock/util.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <event/periodic.h>
#include <thread.h>
#include <arpa/inet.h>
#include <dht.h>
#include <sys/socket.h>
#include <net/gcoap.h>
#include <periph/rtc.h>

#define MAIN_QUEUE_SIZE (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void periodic_event(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_msg_event_t event;
    evtimer_t timer;

    /* Initialize event timer for message-based events */
    evtimer_init_mbox(&timer);

    /* Set up periodic event every 1 second (1,000,000 microseconds) */
    event.event.offset = 1000000;

    /* Add event to timer with callback function */
    evtimer_add_msg(&timer, &event, thread_getpid());

    /* Event loop to run the timer */
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        periodic_event(NULL);
    }

    return 0;
}