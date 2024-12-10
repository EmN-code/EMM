
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char device_state[10];
static udp_socket_t udp_socket;

void send_device_state(void) {
    static uint8_t buffer[10];
    int len = snprintf((char *)buffer, sizeof(buffer), "%s", device_state);
    udp_send(&udp_socket, buffer, len);
}

int main(void) {
    // Initialize the device state
    strncpy(device_state, DEVICE_STATE_WORK, sizeof(device_state));

    // Initialize the UDP socket
    udp_socket_init(&udp_socket);
    udp_socket_connect(&udp_socket, UDP_SERVER_IP, UDP_SERVER_PORT);

    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
