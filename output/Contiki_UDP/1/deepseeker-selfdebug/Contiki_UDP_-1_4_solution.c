
#include "contiki.h"
#include "contiki-net.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *udp_conn;
static struct etimer et;

void send_device_state(const char *state) {
    static char buf[32];
    int len = snprintf(buf, sizeof(buf), "Device state: %s", state);
    uip_udp_packet_send(udp_conn, buf, len);
}

PROCESS(device_state_process, "Device State Process");
AUTOSTART_PROCESSES(&device_state_process);

PROCESS_THREAD(device_state_process, ev, data) {
    static bool working = true;

    PROCESS_BEGIN();

    // Initialize UDP connection
    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, 0, 0, 0, 0, 0, 0, 0, 0); // IPv4 address
    udp_conn = udp_new(&server_ip, UIP_HTONS(UDP_SERVER_PORT), NULL);
    udp_bind(udp_conn, UIP_HTONS(0));

    // Start the periodic timer
    etimer_set(&et, DEVICE_STATE_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if(working) {
            send_device_state("work");
        } else {
            send_device_state("not work");
        }

        // Toggle the state
        working = !working;

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}
