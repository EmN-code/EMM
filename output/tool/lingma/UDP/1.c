#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5000000  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t server_ep;

void send_data() {
    char data[] = "work";  // Change to "not work" as needed
    ssize_t res = sock_udp_send(&sock, data, strlen(data), &server_ep);

    if (res < 0) {
        printf("Failed to send data: %d\n", (int)res);
    } else {
        printf("Sent data: %s\n", data);
    }
}

int main(void) {
    // Initialize the network interface
    sock_udp_ep_t local_ep = SOCK_IPV4_EP_ANY;
    int res = sock_udp_create(&sock, &local_ep, NULL, 0);

    if (res < 0) {
        printf("Failed to create UDP socket: %d\n", res);
        return 1;
    }

    // Set up the server endpoint
    server_ep = (sock_udp_ep_t) {
        .family = AF_INET,
        .port = SERVER_PORT,
        .addr.ipv4 = ipv4_addr_from_str(SERVER_IP)
    };

    if (server_ep.addr.ipv4 == NULL) {
        printf("Invalid server IP address\n");
        return 1;
    }

    // Send data periodically
    while (1) {
        send_data();
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}