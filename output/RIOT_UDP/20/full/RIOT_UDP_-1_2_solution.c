#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define SERVER_PORT 20001
#define CLIENT_BUFFER_SIZE 64

#define SERVER_IPV4 { 47, 102, 103, 1 }

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t server_address[] = SERVER_IPV4;
    size_t tries = 10;

    // Initialize IPV4 address
    while (tries--) {
        if (inet_pton(AF_INET, (const char *)server_address, &remote.addr.ipv4) == 1) {
            printf("IPv4 conversion successful\n");
            break;
        }
        printf("IPv4 conversion failed, retrying...\n");
        xtimer_sleep(1);
    }

    // Check if we exited due to exhausted attempts
    if (tries == 0) {
        puts("IPv4 address conversion failed too many times");
        return -1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP sock");
        return -1;
    }

    char *messages[] = {"work", "not work"};
    unsigned message_index = 0;

    while (1) {
        // Send UDP message every 5 seconds
        if (sock_udp_send(&sock, messages[message_index], strlen(messages[message_index]), &remote) < 0) {
            puts("Error sending UDP message");
        } else {
            printf("Sent message: %s\n", messages[message_index]);
        }

        xtimer_sleep(5);
        message_index = (message_index + 1) % 2; // Toggle between "work" and "not work"
    }

    return 0;
}