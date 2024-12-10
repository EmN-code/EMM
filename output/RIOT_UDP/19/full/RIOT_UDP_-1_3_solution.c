#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>

#define UDP_SERVER "47.102.103.1"
#define UDP_PORT 20001
#define MESSAGE "work"
#define SLEEP_INTERVAL 5
#define RETRY_INTERVAL 1

int main(void) {
    sock_udp_ep_t server_ep;
    sock_udp_t sock;
    int rc;

    // Initialize the server endpoint
    memset(&server_ep, 0, sizeof(server_ep));
    server_ep.family = AF_INET;
    server_ep.port = UDP_PORT;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, UDP_SERVER, &server_ep.addr.ipv4) != 1) {
        puts("Error: unable to convert server address");
        return 1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create a UDP socket");
        return 1;
    }

    while (1) {
        // Send message to the server
        rc = sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &server_ep);

        if (rc < 0) {
            printf("Error sending message: %d. Retrying in %d seconds...\n", rc, RETRY_INTERVAL);
            xtimer_sleep(RETRY_INTERVAL);
            continue;
        }

        printf("Message sent to %s:%d\n", UDP_SERVER, UDP_PORT);
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}