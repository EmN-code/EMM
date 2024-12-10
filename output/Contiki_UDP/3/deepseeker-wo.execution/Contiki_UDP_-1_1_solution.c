
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 12345
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static struct etimer et;

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t datalen)
{
    // Optional: Handle incoming UDP packets if needed
    printf("Received UDP packet from ");
    uip_debug_ipaddr_print(sender_addr);
    printf(":%d\n", sender_port);
}

static void send_device_state(void)
{
    static int state = 0;
    const char *message = state ? "work" : "not work";
    state = !state; // Toggle state

    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, UDP_SERVER_IP, 0, 0, 0, 0, 0, 0, 0);

    simple_udp_sendto(&udp_conn, message, strlen(message), &server_ip);
    printf("Sent: %s\n", message);
}

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);

    // Set up the periodic timer
    etimer_set(&et, SEND_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        send_device_state();

        // Reset the timer to send again in 5 seconds
        etimer_reset(&et);
    }

    PROCESS_END();
}
