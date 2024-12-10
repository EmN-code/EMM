#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define UDP_SEND_INTERVAL (5U * US_PER_SEC)  // Send every 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Converting IP address from text to binary form.
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to convert IP address.");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: cannot create UDP sock.");
        return 1;
    }

    puts("UDP client running.");
    char *message = "Device state: work";

    while (1) {
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            puts("Error: cannot send message.");
        } else {
            puts("Message sent successfully.");
        }
        xtimer_sleep(UDP_SEND_INTERVAL / US_PER_SEC);  // Convert to seconds
    }

    return 0;
}