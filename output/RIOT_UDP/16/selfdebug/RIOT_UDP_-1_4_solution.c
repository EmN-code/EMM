#include <stdio.h>
#include <string.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL (5U * 1000000U)  // Five seconds in microseconds

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t addr[sizeof(struct in_addr)];

    if (inet_pton(AF_INET, SERVER_IP, addr) != 1) {
        printf("Failed to parse IP address\n");
        return 1;
    }
    memcpy(&remote.addr, addr, sizeof(addr));

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    char msg[] = "Device is working";
    while (1) {
        if (sock_udp_send(&sock, msg, sizeof(msg), &remote) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", msg);
        }
        xtimer_usleep(MESSAGE_INTERVAL);
    }
    return 0;
}