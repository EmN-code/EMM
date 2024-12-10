
#include <stdio.h>
#include "net/ipv6/simple-udp.h"
#include "sys/node-id.h"
#include "sys/etimer.h"
#include "sys/log.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001

static struct simple_udp_connection udp_conn;

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t datalen)
{
    LOG_INFO("Received response '%.*s' from ", datalen, (char *)data);
    LOG_INFO_6ADDR(sender_addr);
    LOG_INFO_("\n");
}

int main(void)
{
    uip_ipaddr_t server_addr;
    static char state[] = "work"; // or "not work"

    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                        UDP_SERVER_PORT, udp_rx_callback);

    uip_ipaddr_u32(&server_addr, UIP_HTONL(0x2f666767)); // 47.102.103.1 in hex

    while(1) {
        simple_udp_sendto(&udp_conn, state, sizeof(state), &server_addr);
        etimer_set(&etimer, 5 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etimer));
    }

    return 0;
}
