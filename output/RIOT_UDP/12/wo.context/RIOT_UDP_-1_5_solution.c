#include <ztimer.h>     // For timing functions
#include <stdio.h>       // For printf function
#include <string.h>      // For string manipulation
#include <net/sock/udp.h>   // For UDP socket functions
#include <net/ipv4/addr.h>  // For IPv4 address functions

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT (20001)
#define UPDATE_INTERVAL (5U)  // Update interval of 5 seconds

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[128];
    sock_udp_t sock;
    char *states[] = {"work", "not work"};
    int current_state = 0;  // 0 means "work", 1 means "not work"

    // Initialize sock_udp
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    // Convert the server IP address from text to binary format
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        printf("Error converting IP address\n");
        return -1;
    }

    printf("UDP client sending to %s:%d\n", SERVER_IP, SERVER_PORT);

    // Main loop to send device state every 5 seconds
    while (1) {
        // Toggle the state for demonstration purposes
        current_state = 1 - current_state;
        snprintf((char *)buf, sizeof(buf), "Device state: %s", states[current_state]);

        if (sock_udp_send(&sock, buf, strlen((char *)buf), &server) < 0) {
            printf("Error sending message\n");
            return -1;
        }

        printf("Sent: %s\n", buf);

        // Sleep for the defined update interval
        ztimer_sleep(ZTIMER_MSEC, UPDATE_INTERVAL * 1000U);
    }

    return 0;
}