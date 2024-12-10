#include <string.h>
#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_PORT 20001
#define SERVER_ADDRESS "47.102.103.1"
#define MESSAGE_INTERVAL (5U * 1000000U) // 5 seconds
#define MSG_BUFFER_SIZE 64

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t message[] = "work";
    char addr_str[IPV6_ADDR_MAX_STR_LEN];

    // Initialize UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: Unable to create UDP socket\n");
        return 1;
    }

    // Convert server address
    if (inet_pton(AF_INET, SERVER_ADDRESS, &server.addr.ipv6) != 1) {
        printf("Error: Invalid server address\n");
        return 1;
    }

    // Periodically send message
    while (1) {
        if (sock_udp_send(&sock, message, sizeof(message), &server) < 0) {
            printf("Error: Unable to send message\n");
        } else {
            printf("Message sent to %s:%u\n", SERVER_ADDRESS, SERVER_PORT);
        }
        xtimer_usleep(MESSAGE_INTERVAL);
    }

    return 0;
}
