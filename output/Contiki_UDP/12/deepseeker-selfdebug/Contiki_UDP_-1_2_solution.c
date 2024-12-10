
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "sys/etimer.h"
#include "sys/process.h"
#include "sys/log.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 12345
#define UDP_SERVER_PORT 20001
#define UDP_SERVER_IP "47.102.103.1"

static struct simple_udp_connection udp_conn;

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t datalen)
{
    LOG_INFO("Received response from server\n");
}

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
    static struct etimer et;
    static char state[] = "work";

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);

    etimer_set(&et, 5 * CLOCK_SECOND);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        uip_ipaddr_t server_ip;
        uip_ip6addr(&server_ip, 0x2f, 0x66, 0x67, 0x67, 0x0, 0x0, 0x0, 0x0); // Convert IP to IPv6 format

        simple_udp_sendto(&udp_conn, state, sizeof(state), &server_ip);

        etimer_reset(&et);
    }

    PROCESS_END();
}
