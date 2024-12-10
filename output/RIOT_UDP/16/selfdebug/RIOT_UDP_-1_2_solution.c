#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "Device state: work"
#define INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    uint8_t addr[16];

    // Convert server IP to binary form
    if (inet_pton(AF_INET6, SERVER_IP, &addr) != 1) {
        printf("Error: could not convert IP address\n");
        return -1;
    }
    server.addr.ipv6[0] = addr[0];

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Could not create UDP socket\n");
        return -1;
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &server) < 0) {
            printf("Error: Could not send message\n");
        }
        else {
            printf("Message sent successfully\n");
        }

        xtimer_usleep(INTERVAL);
    }

    return 0;
}