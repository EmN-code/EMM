#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <sys/socket.h>
#include <net/af.h>
#include <stdio.h>
#include <string.h>

// Define the server's IP address and port
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001

// Define the interval for sending messages in seconds
#define SEND_INTERVAL 5

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };

    // Parse the server IP address
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse server address\n");
        return 1;
    }

    // Create the UDP sock
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error: unable to create UDP sock\n");
        return 1;
    }

    // Buffer for the message to be sent
    char *message = "work"; // change to "not work" as needed
    ssize_t res;

    while (1) {
        // Send the message to the server
        res = sock_udp_send(&sock, message, strlen(message), &remote);
        if (res < 0) {
            printf("Error sending message\n");
        } else {
            printf("Sent message: %s\n", message);
        }

        // Wait for the defined interval
        sleep(SEND_INTERVAL);
    }

    // Close the sock
    sock_udp_close(&sock);

    return 0;
}