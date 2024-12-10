
#include "contiki.h"
#include "sys/etimer.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"

static struct simple_udp_connection udp_conn;
static char device_state[10];

PROCESS(device_state_sender, "Device State Sender");
AUTOSTART_PROCESSES(&device_state_sender);

static void udp_send_state(void) {
    static char buffer[20];
    snprintf(buffer, sizeof(buffer), "%s", device_state);
    simple_udp_sendto(&udp_conn, buffer, strlen(buffer), (struct uip_ipaddr *)UDP_SERVER_IP, UDP_SERVER_PORT);
}

PROCESS_THREAD(device_state_sender, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    // Set initial device state
    strncpy(device_state, DEVICE_STATE_WORK, sizeof(device_state));

    // Set timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send device state
        udp_send_state();

        // Toggle device state
        if (strcmp(device_state, DEVICE_STATE_WORK) == 0) {
            strncpy(device_state, DEVICE_STATE_NOT_WORK, sizeof(device_state));
        } else {
            strncpy(device_state, DEVICE_STATE_WORK, sizeof(device_state));
        }

        // Reset timer for 5 seconds
        etimer_reset(&et);
    }

    PROCESS_END();
}
