#define _POSIX_C_SOURCE 200112L  // Required macro for POSIX compatible libraries.

#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)  // send every 5 seconds

static sock_udp_ep_t server;

int main(void) {
    // Define the server endpoint
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv6) != 1) {
        // Error if conversion of the IP address fails
        puts("Error: Invalid server IP address.");
        return 1;
    }
    server.port = SERVER_PORT;

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        // Error if socket creation fails
        puts("Error: Unable to create UDP socket.");
        return 1;
    }

    char message[] = "Device state: work";
    uint32_t last_send = xtimer_now_usec();

    while (1) {
        if (sock_udp_send(&sock, message, sizeof(message), &server) < 0) {
            // Error if sending message fails
            puts("Error: Failed to send UDP message.");
        } else {
            puts("UDP message sent.");
        }

        // Wait for the next send interval
        xtimer_usleep(SEND_INTERVAL - (xtimer_now_usec() - last_send));
        last_send = xtimer_now_usec();
    }

    // Clean up the socket
    sock_udp_close(&sock);
    return 0;
}
