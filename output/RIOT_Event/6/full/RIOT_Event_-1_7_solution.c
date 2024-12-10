#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mbox.h>
#include <msg.h>
#include <kernel_defines.h>
#include <event/periodic.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <dht.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdbool.h>

#define DELAY 1000000  // 1 second

void callback(void *arg) {
    printf("Callback function executed every second.\n");
}

int main(void) {
    /* Initialization code before starting the periodic task */
    printf("Initializing periodic callback every second.\n");

    while (1) {
        usleep(DELAY);  // sleep for the defined DELAY
        callback(NULL); // call the callback function
    }

    return 0;
}