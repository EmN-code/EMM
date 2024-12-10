#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"
#include "msg.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define SEND_INTERVAL (5U * 1000000U)

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char message[] = "work or not work";

    // Convert IP address string to sockaddr
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Error: Unable to create socket\n");
        return 1;
    }

    while (1) {
        // Send message
        if (sock_udp_send(&sock, message, sizeof(message), &remote) < 0) {
            printf("Error: Unable to send message\n");
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait for specified interval
        xtimer_sleep(SEND_INTERVAL / 1000000U);
    }

    // Close the socket
    sock_udp_close(&sock);
    return 0;
}