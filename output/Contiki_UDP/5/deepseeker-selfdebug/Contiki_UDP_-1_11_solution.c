
#include <stdio.h>
#include "xtimer.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"

int main(void)
{
    // Initialize the UDP socket
    udp_socket_t sock;
    udp_socket_init(&sock);

    // Resolve the server IP address
    ipv6_addr_t server_addr;
    if (ipv6_addr_from_str(&server_addr, UDP_SERVER_IP) == NULL) {
        printf("Error: invalid server IP address\n");
        return 1;
    }

    // Bind the socket to a local port (optional for sending)
    udp_socket_bind(&sock, 0);

    // Prepare the message to send
    char message[20];
    snprintf(message, sizeof(message), "Device state: %s", DEVICE_STATE);

    while (1) {
        // Send the message to the server
        ssize_t sent = udp_socket_sendto(&sock, message, strlen(message), &server_addr, UDP_SERVER_PORT);
        if (sent < 0) {
            printf("Error sending UDP message\n");
        } else {
            printf("Sent %zd bytes: %s\n", sent, message);
        }

        // Wait for 5 seconds before sending the next message
        xtimer_sleep(5);
    }

    return 0;
}
