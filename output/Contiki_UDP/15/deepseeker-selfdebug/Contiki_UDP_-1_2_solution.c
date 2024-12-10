
#include <stdio.h>
#include <string.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "xtimer.h"

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 12345
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static struct simple_udp_connection udp_conn;

void send_device_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    char buf[32];
    sprintf(buf, "%s", states[state_index]);
    simple_udp_sendto(&udp_conn, buf, strlen(buf), NULL);

    state_index = 1 - state_index; // Toggle between 0 and 1
}

int main(void) {
    uip_ipaddr_t server_addr;

    // Initialize the UDP connection
    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

    // Resolve the server address
    if (uip_ipaddr_from_str(&server_addr, UDP_SERVER_ADDR) == NULL) {
        puts("Error: unable to resolve server address");
        return 1;
    }

    // Set the remote address for the UDP connection
    uip_ds6_addr_add(&server_addr, 0, ADDR_MANUAL);

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
