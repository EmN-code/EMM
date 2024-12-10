#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"

#define UDP_SERVER_ADDRESS 0x2F660467
#define UDP_SERVER_PORT 20001
#define LOCAL_UDP_PORT 1234

PROCESS(contiki_udp_client_process, "Contiki UDP Client");
AUTOSTART_PROCESSES(&contiki_udp_client_process);

static struct etimer periodic_timer;
static struct uip_udp_conn *conn;

PROCESS_THREAD(contiki_udp_client_process, ev, data) {
    PROCESS_BEGIN();

    uip_ipaddr_t addr;

    // Setup UDP
    conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL);
    if(conn == NULL) {
        printf("Failed to create UDP connection\n");
        PROCESS_EXIT();
    }
    uip_ipaddr(&addr, UDP_SERVER_ADDRESS);
    udp_connect(conn, &addr, UIP_HTONS(UDP_SERVER_PORT));

    // Set the timer to fire every 5 seconds
    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        const char *msg;
        msg = (clock_time() / CLOCK_SECOND % 2 == 0) ? "Device is working" : "Device is not working";

        printf("Sending UDP message: %s\n", msg);
        uip_udp_packet_send(conn, msg, strlen(msg));

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}