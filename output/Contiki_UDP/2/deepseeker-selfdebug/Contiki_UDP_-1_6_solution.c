
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/rtimer.h"
#include "sys/ctimer.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"

#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *udp_conn;
static char device_state[] = "work";

PROCESS(device_state_process, "Device State Process");
AUTOSTART_PROCESSES(&device_state_process);

void send_device_state(void) {
    static char buf[32];
    sprintf(buf, "%s", device_state);
    uip_udp_packet_send(udp_conn, buf, strlen(buf));
}

PROCESS_THREAD(device_state_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
    udp_bind(udp_conn, UIP_HTONS(0));
    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, UDP_SERVER_IP);
    udp_connect(udp_conn, &server_ip, UIP_HTONS(UDP_SERVER_PORT));

    etimer_set(&et, DEVICE_STATE_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        send_device_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&device_state_process, NULL);
    return 0;
}
