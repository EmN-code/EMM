#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote;
    char message[] = "work";
    uint8_t addr[sizeof(struct in_addr)];

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &addr) != 1) {
        puts("Error: unable to parse IP address");
        return 1;
    }
    memcpy(&remote.addr.ipv4, addr, sizeof(struct in_addr));

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create socket");
        return 1;
    }

    // Infinite loop to send data every five seconds
    while (1) {
        if (sock_udp_send(&sock, message, sizeof(message), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent");
        }
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}