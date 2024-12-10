#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT (20001)
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    // Setting up the remote server endpoint for UDP
    sock_udp_ep_t server = {.family = AF_INET, .port = SERVER_PORT};
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    // Create UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket\n");
        return 1;
    }

    // Buffers
    char *message_work = "work";
    char *message_not_work = "not work";
    char buffer[128];

    while (1) {
        int send_result = sock_udp_send(&sock, message_work, strlen(message_work), &server);
        if (send_result < 0) {
            printf("Error sending UDP packet: %d\n", send_result);
        } else {
            printf("Successfully sent '%s' to %s:%d\n", message_work, SERVER_IP, SERVER_PORT);
        }

        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}