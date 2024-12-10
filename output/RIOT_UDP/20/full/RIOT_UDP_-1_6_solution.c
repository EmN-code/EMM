#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define SLEEP_INTERVAL 5  // seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    int rc;
    
    // Translate IP address from string to network format
    rc = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (rc != 1) {
        puts("Error: invalid IP address format");
        return -1;
    }
    
    sock_udp_t sock;
    rc = sock_udp_create(&sock, NULL, NULL, 0);
    if (rc < 0) {
        printf("Error: unable to create UDP socket (%d)\n", rc);
        return -1;
    }

    while (1) {
        while (sock_udp_send(&sock, DEVICE_STATE, strlen(DEVICE_STATE), &remote) < 0) {
            printf("Failed to send data, retrying...\n");
            xtimer_sleep(1);
        }
        printf("State '%s' sent successfully\n", DEVICE_STATE);
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}