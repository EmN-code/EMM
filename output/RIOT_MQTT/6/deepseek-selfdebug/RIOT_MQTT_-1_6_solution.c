
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t server_addr = SOCK_IPV4_EP_ANY;
    char message[] = "work or not work";
    uint8_t ip_addr[4];

    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, ip_addr) <= 0) {
        puts("Invalid address/ Address not supported");
        return 1;
    }

    // Set the server address
    server_addr.family = AF_INET;
    server_addr.port = SERVER_PORT;
    memcpy(&server_addr.addr.ipv4, ip_addr, sizeof(ip_addr));

    while (1) {
        // Send the message to the server
        if (sock_udp_send(&sock, message, strlen(message), &server_addr) < 0) {
            puts("Error sending message");
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait for 5 seconds
        xtimer_usleep(MSG_INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
