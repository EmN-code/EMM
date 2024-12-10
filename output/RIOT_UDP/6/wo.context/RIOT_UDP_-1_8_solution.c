#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL_USEC (5U * US_PER_SEC)

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    char buffer[64];
    size_t len;
    int res;

    // Convert the string representation of the IP address to its binary form
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error: can't parse IP address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP sock");
        return 1;
    }

    // Prepare the message
    strcpy(buffer, "work");  // or "not work" depending on the state
    len = strlen(buffer);

    // Send the message every 5 seconds
    while (1) {
        if (sock_udp_send(&sock, buffer, len, &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent");
        }

        // Wait for 5 seconds
        xtimer_usleep(MESSAGE_INTERVAL_USEC);
    }

    return 0;
}
