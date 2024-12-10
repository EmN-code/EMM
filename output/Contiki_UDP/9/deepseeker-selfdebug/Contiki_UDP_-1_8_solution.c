
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/simple_udp.h"
#include "net/ipv6/addr.h"

#define UDP_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define DELAY_MS (5 * 1000 * 1000U)  // 5 seconds

static struct simple_udp_connection udp_conn;

int main(void)
{
    // Initialize the UDP connection
    simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);

    // IP address of the UDP server
    uip_ipaddr_t server_addr;
    uip_ipaddr(&server_addr, 47, 102, 103, 1);

    char state[10];
    strncpy(state, DEVICE_STATE, sizeof(state));

    while (1) {
        // Send the device state to the UDP server
        simple_udp_sendto(&udp_conn, state, strlen(state), &server_addr);

        // Wait for 5 seconds
        xtimer_usleep(DELAY_MS);
    }

    return 0;
}
