
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
    static uint8_t buf[16];
    int res;

    udp_socket_init(&sock);
    udp_socket_bind(&sock, 0);

    while (1) {
        sprintf((char *)buf, "%s", DEVICE_STATE);
        res = udp_socket_sendto(&sock, buf, strlen((char *)buf), UDP_SERVER_IP, UDP_SERVER_PORT);
        if (res < 0) {
            printf("Failed to send UDP packet\n");
        } else {
            printf("Sent device state: %s\n", DEVICE_STATE);
        }

        xtimer_usleep(SEND_INTERVAL);
    }
}

int main(void) {
    puts("Starting UDP client...");
    udp_send_state();
    return 0;
}
