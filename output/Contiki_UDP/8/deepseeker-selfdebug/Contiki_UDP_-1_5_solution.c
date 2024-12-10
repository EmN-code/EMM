
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
#include "sys/etimer.h"
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_device_state(const char *state) {
    static char buf[32];
    int len = snprintf(buf, sizeof(buf), "Device state: %s", state);
    uip_udp_packet_send(conn, buf, len);
}

PROCESS_THREAD(udp_client_process, ev, data) {
    static char state[] = "work";

    PROCESS_BEGIN();

    // Initialize UDP connection
    conn = uip_udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT));
    if (conn != NULL) {
        uip_ipaddr_t addr;
        uip_ipaddr(&addr, UDP_SERVER_IP, UDP_SERVER_PORT);
        uip_udp_bind(conn, UIP_HTONS(0));
        uip_udp_connect(conn, &addr, UIP_HTONS(UDP_SERVER_PORT));
    }

    // Set the timer for sending state every 5 seconds
    etimer_set(&et, SEND_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state
        send_device_state(state);

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}
