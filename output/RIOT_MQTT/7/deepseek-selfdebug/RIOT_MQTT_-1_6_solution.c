
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MSG_SIZE 128
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char message[MSG_SIZE];

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_ep_t remote;
    uint8_t ip_addr[4];

    // Convert IP address string to byte array
    if (inet_pton(AF_INET, SERVER_IP, ip_addr) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Set up the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    memcpy(&remote.addr.ipv4[0], ip_addr, 4);

    // Create a TCP socket
    if (sock_tcp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating socket");
        return 1;
    }

    // Connect to the MQTT broker
    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Error connecting to server");
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        // Prepare the message
        snprintf(message, MSG_SIZE, "work or not work");

        // Send the message
        if (sock_tcp_send(&sock, message, strlen(message), 0) < 0) {
            puts("Error sending message");
            break;
        }

        // Print the sent message
        printf("Sent: %s\n", message);

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
