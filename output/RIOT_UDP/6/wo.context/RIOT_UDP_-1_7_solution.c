#include <xtimer.h>
#include <net/sock/udp.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void)
{
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = SERVER_PORT
    };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse UDP server address");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return -1;
    }

    char *device_state = "work";

    while (1) {
        ssize_t res = sock_udp_send(&sock, device_state, strlen(device_state), &remote);
        if (res < 0) {
            puts("Error: unable to send data");
        } else {
            puts("Data sent successfully");
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}