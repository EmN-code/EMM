
#include <stdio.h>
#include "net/ipv6/simple-udp.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static struct simple_udp_connection udp_conn;

void udp_send_state(void) {
    static char state[] = DEVICE_STATE;
    uip_ipaddr_t server_addr;
    uip_ipaddr(&server_addr, UDP_SERVER_IP, 0, 0, 0, 0, 0, 0, 0);
    simple_udp_sendto(&udp_conn, state, sizeof(state) - 1, &server_addr);
}

int main(void) {
    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    while (1) {
        udp_send_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
