
#include "xtimer.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-udp-packet.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char device_state[10];

void send_udp_message(void) {
    static udp_socket_t sock;
    static struct sockaddr_in server_addr;

    udp_socket_create(&sock, NULL, NULL);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_aton(UDP_SERVER_IP, &server_addr.sin_addr);

    udp_socket_sendto(&sock, device_state, strlen(device_state), (struct sockaddr *)&server_addr, sizeof(server_addr));
    udp_socket_close(&sock);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Simulate device state change
        if (strcmp(device_state, DEVICE_STATE_WORK) == 0) {
            strcpy(device_state, DEVICE_STATE_NOT_WORK);
        } else {
            strcpy(device_state, DEVICE_STATE_WORK);
        }

        send_udp_message();

        xtimer_periodic_wakeup(&last_wakeup, SEND_INTERVAL);
    }

    return 0;
}
