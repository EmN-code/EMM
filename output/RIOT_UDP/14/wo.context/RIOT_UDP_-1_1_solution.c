#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <net/udp.h>
#include <xtimer.h>

#define SERVER_PORT    (20001)
#define CLIENT_PORT    (8888)
#define SERVER_ADDRESS "47.102.103.1"
#define DEVICE_STATE   "work"
#define INTERVAL       (5000000U)  // 5 seconds in microseconds

static udp_t udp_conn;
static struct sockaddr_in6 server_addr;

int main(void) {
    // Initialize UDP connection
    udp_init(&udp_conn);
    
    // Define the server address
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET6, SERVER_ADDRESS, &(server_addr.sin6_addr));
    
    char message[64];
    snprintf(message, sizeof(message), "%s", DEVICE_STATE);
    
    printf("UDP client example started.\n");

    while (1) {
        // Connect to server and send the message
        if (udp_connect(&udp_conn, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printf("Could not connect.\n");
            continue;
        }
        ssize_t res = udp_send(&udp_conn, message, strlen(message));
        if (res < 0) {
            printf("Error sending data: %d\n", res);
        } else {
            printf("Sent %zd bytes.\n", res);
        }
        
        // Wait for 5 seconds
        xtimer_sleep(INTERVAL / 1000000);
    }

    return 0;
}