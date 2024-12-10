
#include <stdio.h>
#include <string.h>
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-nameserver.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static struct simple_udp_connection udp_conn;

void udp_send_state(void) {
    char message[32];
    snprintf(message, sizeof(message), "%s", DEVICE_STATE_MESSAGE);
    uip_ipaddr_t server_addr;
    uip_ipaddr(&server_addr, 47, 102, 103, 1);
    simple_udp_sendto(&udp_conn, message, strlen(message), &server_addr);
}

int main(void) {
    // Initialize the RIOT kernel
    xtimer_init();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    while (1) {
        udp_send_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
