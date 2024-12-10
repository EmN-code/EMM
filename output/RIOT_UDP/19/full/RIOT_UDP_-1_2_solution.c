#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U) // Send every 5 seconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    // Converts the IP address
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }

    char *message = "Device State: Work";
    while (1) {
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &server);
        if (res < 0) {
            puts("Error sending message, retrying...");
            xtimer_sleep(1); // Wait 1 sec and retry
            continue;
        }
        printf("Message sent: %s\n", message);
        xtimer_sleep(SEND_INTERVAL);
    }
    return 0;
}