
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

int main(void)
{
    // Initialize the UDP socket
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 12345;  // Local port to bind to

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Convert the server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    char message[] = "work";  // Example device state message

    while (1) {
        // Send the message to the server
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            puts("Error sending message");
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
