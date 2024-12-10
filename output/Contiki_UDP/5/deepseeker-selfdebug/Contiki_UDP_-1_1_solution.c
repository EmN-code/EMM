
#include "contiki.h"
#include "sys/etimer.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "dev/leds.h"
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *udp_conn;
static struct etimer et;

PROCESS(udp_sender_process, "UDP Sender Process");
AUTOSTART_PROCESSES(&udp_sender_process);

void udp_send_state(void) {
    static char buf[10];
    strncpy(buf, DEVICE_STATE_MESSAGE, sizeof(buf));
    uip_ipaddr_t ipaddr;
    uip_ipaddr(&ipaddr, UDP_SERVER_IP);
    uip_udp_packet_send(udp_conn, buf, strlen(buf));
}

PROCESS_THREAD(udp_sender_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize UDP connection
    udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
    uip_ipaddr_t ipaddr;
    uip_ipaddr(&ipaddr, UDP_SERVER_IP);
    udp_bind(udp_conn, UIP_HTONS(UDP_SERVER_PORT));

    // Start periodic timer
    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send device state
        udp_send_state();

        // Reset timer
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize Contiki
    process_init();
    process_start(&udp_sender_process, NULL);
    autostart_start(autostart_processes);

    // Main loop
    while(1) {
        process_run();
    }

    return 0;
}
