
#include "xtimer.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-udp-packet.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void send_udp_message(void) {
    static char message[128];
    udp_socket_t sock;
    sockaddr_in_t server_addr;

    udp_socket_create(&sock);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_aton(UDP_SERVER_IP, &server_addr.sin_addr);

    snprintf(message, sizeof(message), "Device state: %s", DEVICE_STATE);
    udp_socket_sendto(&sock, message, strlen(message), &server_addr);
    udp_socket_close(&sock);
}

int main(void) {
    while (1) {
        send_udp_message();
        xtimer_usleep(SEND_INTERVAL);
    }
    return 0;
}
