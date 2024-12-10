
#include "xtimer.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static struct simple_udp_connection udp_conn;

void send_device_state(void) {
    char buffer[] = DEVICE_STATE;
    simple_udp_sendto(&udp_conn, buffer, sizeof(buffer), UDP_SERVER_IP, UDP_SERVER_PORT);
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
