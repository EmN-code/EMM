#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <thread.h>
#include <sock/udp.h>
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // 5 seconds
#define BUFFER_SIZE 64

static char udp_buffer[BUFFER_SIZE];
sock_udp_ep_t remote = {.family = AF_INET, .port = SERVER_PORT};

int main(void) 
{
    // Convert server IP address
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket\n");
        return 1;
    }

    while (1) {
        // Prepare the device state message
        const char *msg = "Device is working"; // You can change to "not work" as needed
        strcpy(udp_buffer, msg);

        // Send message
        if (sock_udp_send(&sock, udp_buffer, strlen(udp_buffer), &remote) < 0) {
            puts("Error sending message\n");
        }
        else {
            printf("Sent: %s\n", msg);
        }

        // Wait for the next interval
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    // Clean up, though we'll never reach here
    sock_udp_close(&sock);
    return 0;
}