#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000UL) // 5 seconds in microseconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buffer[] = "work or not work";
    sock_udp_t sock;

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }
    
    // Convert the IP address to the binary form
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        puts("Error: Invalid address");
        return -1;
    }

    while (1) {
        // Send the state "work or not work" to the server
        if (sock_udp_send(&sock, buffer, sizeof(buffer), &remote) < 0) {
            puts("Error sending message");
        }

        // Wait for the specified interval
        xtimer_sleep(5);
    }

    return 0;
}