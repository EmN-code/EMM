
#include <stdio.h>
#include "net/ipv6/simple-udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_SECONDS 5

static struct simple_udp_connection udp_conn;

void send_device_state(void) {
    char buffer[] = DEVICE_STATE;
    simple_udp_sendto(&udp_conn, buffer, sizeof(buffer), SERVER_IP, SERVER_PORT);
}

int main(void) {
    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, 0, NULL);

    while (1) {
        send_device_state();
        xtimer_sleep(INTERVAL_SECONDS);
    }

    return 0;
}