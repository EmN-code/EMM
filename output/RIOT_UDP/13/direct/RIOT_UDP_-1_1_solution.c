#include <ztimer.h>
#include "net/sock/udp.h"
#include "net/af.h"
#include <stdio.h>

#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static const char server_addr_str[] = "47.102.103.1";
static int device_state = 0;  // 0 for 'not work', 1 for 'work'

void send_device_state(sock_udp_ep_t *remote) {
    /* Buffer to hold the message to be sent */
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "state: %s", device_state ? "work" : "not work");

    printf("Sending device state: %s\n", buffer);
    
    if (sock_udp_send(NULL, buffer, strlen(buffer), remote) < 0) {
        puts("Error sending UDP packet");
    }
}

int main(void) {
    /* Initialize the remote server endpoint structure */
    sock_udp_ep_t remote = { .family = AF_INET,
                             .port = SERVER_PORT };
    
    /* Parse the server's IP address and assign it to the endpoint */
    if (ipv6_addr_from_str(&remote.addr.ipv6, server_addr_str) == NULL) {
        puts("Error: unable to parse server address");
        return -1;
    }

    puts("Starting UDP client example for ESP32...");

    /* Periodically send the device state to the server */
    while (1) {
        send_device_state(&remote);
        ztimer_sleep(ZTIMER_USEC, SEND_INTERVAL);
    }

    return 0;
}