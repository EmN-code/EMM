#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // Include header for inet_pton
#include <xtimer.h>     // Include RIOT timer header for delaying
#include <net/sock/udp.h>

#define SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"
#define MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    printf("Starting UDP client example...\n");

    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: cannot convert server IP\n");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create socket\n");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &remote) < 0) {
            printf("Error: unable to send message\n");
            break;
        }
        printf("Message sent: %s\n", MESSAGE);
        xtimer_usleep(INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}