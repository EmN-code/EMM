
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 12345
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t datalen)
{
    printf("Received response from server\n");
}

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer et;
    static char state[] = "work"; // or "not work"

    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                        UDP_SERVER_PORT, udp_rx_callback);

    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the device state to the server
        simple_udp_sendto(&udp_conn, state, strlen(state),
                          uip_ip6addr(UDP_SERVER_IP, 0, 0, 0, 0, 0, 0, 0));

        printf("Sent state: %s\n", state);

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize the Contiki system
    process_init();
    process_start(&udp_client_process, NULL);
    autostart_start(autostart_processes);

    // Main loop
    while(1) {
        process_run();
    }

    return 0;
}
