
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "dev/serial-line.h"
#include "sys/log.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 12345

static struct uip_udp_conn *udp_conn;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static void udp_send_state(const char *state) {
    static char buf[32];
    int len = snprintf(buf, sizeof(buf), "Device state: %s", state);
    uip_udp_packet_send(udp_conn, buf, len);
}

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer et;
    static char state[] = "work";

    PROCESS_BEGIN();

    // Initialize the UDP connection
    udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
    udp_bind(udp_conn, UIP_HTONS(UDP_CLIENT_PORT));
    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, UDP_SERVER_IP);
    uip_udp_conn_remote(udp_conn, &server_ip, UIP_HTONS(UDP_SERVER_PORT));

    // Set the timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state
        udp_send_state(state);

        // Toggle the state
        if (strcmp(state, "work") == 0) {
            strcpy(state, "not work");
        } else {
            strcpy(state, "work");
        }

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize Contiki
    contiki_init();

    // Start the Contiki processes
    process_start(&udp_client_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
