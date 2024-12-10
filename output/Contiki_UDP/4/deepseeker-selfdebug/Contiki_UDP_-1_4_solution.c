
#include "xtimer.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/udp-socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS 5000

static void udp_send_state(void) {
    static char buffer[16];
    udp_socket_t sock;
    udp_socket_init(&sock);
    udp_socket_connect(&sock, SERVER_IP, SERVER_PORT);
    snprintf(buffer, sizeof(buffer), "%s", DEVICE_STATE);
    udp_socket_send(&sock, buffer, strlen(buffer));
    udp_socket_close(&sock);
}

int main(void) {
    while (1) {
        udp_send_state();
        xtimer_sleep(INTERVAL_MS / 1000);
    }
    return 0;
}
