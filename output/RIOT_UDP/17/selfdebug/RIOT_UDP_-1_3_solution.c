#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U)  // 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t state = 1;  // 1 for 'work', 0 for 'not work'
    char msg[16];
    snprintf(msg, sizeof(msg), "state: %s", state ? "work" : "not work");

    // Convert IP address in string format to a parsed IPv4 address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: invalid IP address\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP sock\n");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, msg, strlen(msg), &remote) < 0) {
            printf("Error: unable to send message\n");
        } else {
            printf("Sent: %s\n", msg);
        }
        // Sleep for SEND_INTERVAL microseconds
        xtimer_usleep(SEND_INTERVAL);
    }

    return 0;
}