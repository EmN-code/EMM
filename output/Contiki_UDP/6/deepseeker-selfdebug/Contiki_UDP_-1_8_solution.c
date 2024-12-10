
#include <stdio.h>
#include "net/ipv6/simple-udp.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static struct simple_udp_connection udp_conn;

static void send_device_state(void) {
    char state_msg[20];
    snprintf(state_msg, sizeof(state_msg), "Device state: %s", DEVICE_STATE);
    uip_ipaddr_t server_addr;
    uip_ipaddr(&server_addr, UDP_SERVER_IP);
    simple_udp_sendto(&udp_conn, state_msg, strlen(state_msg), &server_addr);
}

int main(void) {
    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
