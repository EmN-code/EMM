#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv6/addr.h>

#include "net/sock/udp.h"

#define SERVER_IP        "47.102.103.1"
#define SERVER_PORT      (20001)
#define SEND_INTERVAL    (5U)

int main(void)
{
    sock_udp_ep_t remote;
    sock_udp_t sock;
    char message[] = "Device state: work"; // or "Device state: not work"

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }
    
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Could not create UDP sock\n");
        return -1;
    }

    while (1) {
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            printf("Error sending message\n");
        }
        else {
            printf("Message sent: %s\n", message);
        }

        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}