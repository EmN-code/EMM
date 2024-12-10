
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void udp_send_state(void) {
    static char buffer[128];
    udp_socket_t sock;
    struct sockaddr_in server_addr;

    udp_socket_init(&sock);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_aton(UDP_SERVER_IP, &server_addr.sin_addr);

    snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

    udp_socket_sendto(&sock, buffer, strlen(buffer), (struct sockaddr *)&server_addr, sizeof(server_addr));
    udp_socket_close(&sock);
}

int main(void) {
    while (1) {
        udp_send_state();
        xtimer_usleep(SEND_INTERVAL);
    }
    return 0;
}
