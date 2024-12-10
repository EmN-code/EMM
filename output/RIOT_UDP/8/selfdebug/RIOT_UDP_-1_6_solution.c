#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char server_addr[] = "47.102.103.1";
    uint8_t addr_bytes[4];
    char message_work[] = "work";
    // Suppose we also want to send the 'not work' message
    char message_not_working[] = "not work";

    inet_pton(AF_INET, server_addr, &server.addr.ipv4);  // Convert address from string

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        if (sock_udp_send(&sock, message_work, sizeof(message_work), &server) < 0) {
            puts("Error sending message");
        } else {
            puts("Message sent: work");
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);

        if (sock_udp_send(&sock, message_not_working, sizeof(message_not_working), &server) < 0) {
            puts("Error sending message");
        } else {
            puts("Message sent: not work");
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}