#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <contiki.h>
#include "net/ipv6/uiplib.h"

#define UDP_IP "47.102.103.1"
#define UDP_PORT 20001

static struct etimer et;
static char device_state[10];

PROCESS(udp_sender_process, "UDP sender process");
AUTOSTART_PROCESSES(&udp_sender_process);

PROCESS_THREAD(udp_sender_process, ev, data) {
    PROCESS_BEGIN();

    uip_ipaddr_t server_ipaddr;
    uiplib_ipaddrconv(UDP_IP, &server_ipaddr);

    etimer_set(&et, CLOCK_SECOND * 5);
    strcpy(device_state, "work");  // or "not work"

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (strcmp(device_state, "work") == 0) {
            printf("Device state: %s\n", device_state);
        }
        else if (strcmp(device_state, "not work") == 0) {
            printf("Device state: %s\n", device_state);
        }

        // Send UDP packet here
        printf("Sending UDP packet to %s:%d\n", UDP_IP, UDP_PORT);

        etimer_reset(&et);
    }

    PROCESS_END();
}