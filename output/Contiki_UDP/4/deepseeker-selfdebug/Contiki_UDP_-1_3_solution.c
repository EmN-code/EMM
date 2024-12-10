
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char device_state[10];
static simple_udp_connection_t udp_conn;

void send_device_state(void) {
    static uint8_t state = 0; // 0 for "work", 1 for "not work"
    if (state == 0) {
        strcpy(device_state, DEVICE_STATE_WORK);
        state = 1;
    } else {
        strcpy(device_state, DEVICE_STATE_NOT_WORK);
        state = 0;
    }

    simple_udp_sendto(&udp_conn, device_state, strlen(device_state), UDP_SERVER_IP, UDP_SERVER_PORT);
}

int main(void) {
    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, 0, NULL, NULL);

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
