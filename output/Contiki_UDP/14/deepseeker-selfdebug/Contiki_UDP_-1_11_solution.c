
#include <stdio.h>
#include <string.h>
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "xtimer.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void udp_send_state(void) {
    static char buffer[128];
    udp_socket_t sock;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_aton(UDP_SERVER_IP, &server_addr.sin_addr);

    udp_socket_create(&sock, NULL, NULL, 0);

    while (1) {
        strncpy(buffer, DEVICE_STATE_MESSAGE, sizeof(buffer) - 1);
        udp_socket_sendto(&sock, buffer, strlen(buffer), (struct sockaddr *)&server_addr, sizeof(server_addr));
        xtimer_usleep(SEND_INTERVAL);
    }

    udp_socket_close(&sock);
}

int main(void) {
    puts("Starting UDP client...");
    udp_send_state();
    return 0;
}
