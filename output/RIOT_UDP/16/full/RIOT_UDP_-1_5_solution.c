#include <xtimer.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5 // interval in seconds between sends

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    char *message = "work";
    int retries;
    int rc;

    // Convert the server IP address
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Failed to create UDP sock\n");
        return 1;
    }

    while (1) {
        retries = 0;
        do {
            printf("Sending state to server\n");
            rc = sock_udp_send(&sock, message, strlen(message), &remote);
            if(rc < 0) {
                printf("Send failed: %d. Retrying...\n", rc);
                xtimer_sleep(1);
            }
        } while (rc < 0 && retries++ < 3);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}