#include "net/routing/routing.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "net/mac/framer/framer.h"
#include "simple-udp.h"
#include "uip-ds6.h"
#include "uip.h"

#include "contiki.h"
#include "contiki-net.h"

// UDP connection settings
#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND) // Send data every 5 seconds

PROCESS(nrf52840_udp_client_process, "NRF52840 UDP Client Process");
AUTOSTART_PROCESSES(&nrf52840_udp_client_process);

static struct simple_udp_connection udp_conn;

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t datalen) {
    // Handle incoming UDP packet
    printf("Received UDP packet from %d.%d.%d.%d:%d\n",
           sender_addr->u8[0], sender_addr->u8[1], sender_addr->u8[2], sender_addr->u8[3],
           sender_port);
}

PROCESS_THREAD(nrf52840_udp_client_process, ev, data) {
    static struct etimer timer;
    static char state_message[20];
    uip_ipaddr_t server_ipaddr;

    PROCESS_BEGIN();

    // Resolve server address
    uiplib_ipaddrconv(UDP_SERVER_IP, &server_ipaddr);

    // Register UDP connection and callback
    simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);

    // Set up the periodic timer
    etimer_set(&timer, SEND_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);

        // Prepare the state message
        snprintf(state_message, sizeof(state_message), "Device state: work");

        // Send UDP packet
        simple_udp_sendto(&udp_conn, state_message, strlen(state_message), &server_ipaddr);

        printf("Sent UDP packet to %d.%d.%d.%d:%d\n",
               server_ipaddr.u8[0], server_ipaddr.u8[1], server_ipaddr.u8[2], server_ipaddr.u8[3],
               UDP_SERVER_PORT);
    }

    PROCESS_END();
}
