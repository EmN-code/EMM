
#include <stdio.h>
#include <xtimer.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void udp_send_state(void) {
    udp_socket_t sock;
    uint8_t buffer[16];
    int res;

    udp_socket_create(&sock, NULL, NULL, 0);
    res = udp_socket_connect(&sock, UDP_SERVER_IP, UDP_SERVER_PORT);
    if (res < 0) {
        printf("Error connecting to UDP server\n");
        return;
    }

    snprintf((char *)buffer, sizeof(buffer), "%s", DEVICE_STATE);
    res = udp_socket_send(&sock, buffer, strlen((char *)buffer), NULL, 0);
    if (res < 0) {
        printf("Error sending UDP packet\n");
    } else {
        printf("Sent device state: %s\n", DEVICE_STATE);
    }

    udp_socket_close(&sock);
}

int main(void) {
    while (1) {
        udp_send_state();
        xtimer_usleep(INTERVAL);
    }
    return 0;
}
