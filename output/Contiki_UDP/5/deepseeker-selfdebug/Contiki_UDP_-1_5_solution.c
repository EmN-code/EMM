
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void udp_send_state(void) {
    static udp_socket_t sock;
    static char buffer[16];
    int res;

    udp_socket_init(&sock);
    udp_socket_connect(&sock, UDP_SERVER_IP, UDP_SERVER_PORT);

    strncpy(buffer, DEVICE_STATE, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    res = udp_socket_send(&sock, buffer, strlen(buffer));
    if (res < 0) {
        printf("Failed to send UDP packet\n");
    } else {
        printf("Sent device state: %s\n", buffer);
    }

    udp_socket_close(&sock);
}

int main(void) {
    while (1) {
        udp_send_state();
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}
