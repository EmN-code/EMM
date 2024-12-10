#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t server = { .port = SERVER_PORT };
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("Error: unable to parse server IP address");
        return -1;
    }

    while (1) {
        sock_udp_send(&sock, DEVICE_STATE, sizeof(DEVICE_STATE), &server);
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}