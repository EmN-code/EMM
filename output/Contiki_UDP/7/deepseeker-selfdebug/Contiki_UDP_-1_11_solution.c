
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include "sys/log.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"

static struct uip_udp_conn *udp_conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void udp_send_state(void) {
    static char buf[] = DEVICE_STATE_MESSAGE;
    uip_ipaddr_t ipaddr;

    uip_ipaddr(&ipaddr, UDP_SERVER_IP);
    uip_udp_packet_send(udp_conn, buf, sizeof(buf) - 1);
    LOG_INFO("Sent device state: %s\n", buf);
}

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
    udp_bind(udp_conn, UIP_HTONS(0));

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        udp_send_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}
