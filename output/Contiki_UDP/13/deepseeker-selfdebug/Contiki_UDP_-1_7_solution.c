
#include <stdio.h>
#include "xtimer.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char device_state[] = DEVICE_STATE_WORK;
static simple_udp_connection_t udp_conn;

void send_device_state(void) {
    if (simple_udp_send(&udp_conn, device_state, sizeof(device_state)) < 0) {
        puts("Error sending UDP message");
    } else {
        puts("Device state sent");
    }
}

int main(void) {
    // Initialize UDP connection
    if (simple_udp_connect(&udp_conn, UDP_SERVER_IP, UDP_SERVER_PORT) < 0) {
        puts("Error connecting to UDP server");
        return 1;
    }

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
