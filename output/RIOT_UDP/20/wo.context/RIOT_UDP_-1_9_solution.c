#include <string.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void)
{
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    // Convert IP address to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse server address");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return -1;
    }

    while (1) {
        char state_msg[] = "work"; // Example state message
        if (sock_udp_send(&sock, state_msg, sizeof(state_msg), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent");
        }

        xtimer_sleep(INTERVAL / US_PER_SEC); // WAIT for 5 seconds
    }

    return 0;
}