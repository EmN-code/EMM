
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static struct simple_udp_connection udp_conn;

void send_device_state(void) {
    char buffer[32];
    sprintf(buffer, "%s", DEVICE_STATE_MESSAGE);
    simple_udp_sendto(&udp_conn, buffer, strlen(buffer), NULL);
}

int main(void) {
    uip_ipaddr_t server_addr;
    uip_ipaddr(&server_addr, UDP_SERVER_IP, UDP_SERVER_PORT);

    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
