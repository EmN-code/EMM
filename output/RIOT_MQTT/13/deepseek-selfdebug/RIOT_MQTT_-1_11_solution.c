
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MSG_SIZE 20
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_t server_addr;
    char msg[MSG_SIZE] = "work or not work";

    // Convert server address string to ipv4_addr_t
    if (ipv4_addr_from_str(&server_addr, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return 1;
    }
    remote.addr.ipv4 = server_addr;

    // Create UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        // Send message
        if (sock_udp_send(&sock, msg, strlen(msg), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent");
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close socket
    sock_udp_close(&sock);

    return 0;
}
