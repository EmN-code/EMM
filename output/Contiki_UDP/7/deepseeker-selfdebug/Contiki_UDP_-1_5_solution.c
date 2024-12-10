
#include <stdio.h>
#include <xtimer.h>
#include <net/ipv6/simple-udp.h>
#include <net/ipv6/uip-udp-packet.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static struct simple_udp_connection udp_conn;

void send_device_state(const char *state) {
    char buf[32];
    sprintf(buf, "Device state: %s", state);
    simple_udp_sendto(&udp_conn, buf, strlen(buf) + 1, (uip_ipaddr_t *)UDP_SERVER_IP, UDP_SERVER_PORT);
}

int main(void) {
    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

    while (1) {
        // Send "work" state
        send_device_state(DEVICE_STATE_WORK);
        xtimer_usleep(SEND_INTERVAL);

        // Send "not work" state
        send_device_state(DEVICE_STATE_NOT_WORK);
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
