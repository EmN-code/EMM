#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

int main(void) {
    // UDP server details
    const char *server_ip = "47.102.103.1";
    uint16_t server_port = 20001;

    // Socket and endpoint structure
    sock_udp_t sock;
    sock_udp_ep_t remote;

    // Message to send
    const char *msg = "Device is working";

    // Initialize the server endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = server_port;

    // Convert the IP address from string to binary form
    if (inet_pton(AF_INET, server_ip, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse server IP");
        return 1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create socket");
        return 1;
    }

    puts("UDP client running");

    while (1) {
        // Send message to server
        if (sock_udp_send(&sock, msg, strlen(msg), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent");
        }
        
        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    // This point will never be reached
    return 0;
}