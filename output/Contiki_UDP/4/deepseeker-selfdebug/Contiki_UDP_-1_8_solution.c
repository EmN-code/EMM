
#include <stdio.h>
#include <string.h>
#include "net/ipv6/simple-udp.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static struct simple_udp_connection udp_conn;

void send_device_state(void) {
    char message[16];
    strncpy(message, DEVICE_STATE_MESSAGE, sizeof(message));
    simple_udp_sendto(&udp_conn, message, strlen(message), NULL, UDP_SERVER_PORT);
}

int main(void) {
    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    // Main loop
    while (1) {
        send_device_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
