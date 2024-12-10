#include "net/emcute.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"

#define UDP_SERVER_ADDR "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define BUFFER_SIZE 128

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = UDP_SERVER_PORT };
    char buffer[BUFFER_SIZE];
    int counter = 0;

    // Parse the server address
    if (sock_udp_str2ep(&remote, UDP_SERVER_ADDR) < 0) {
        perror("Failed to parse server address");
        return -1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        perror("Failed to create UDP socket");
        return -1;
    }

    while (1) {
        const char *state = (counter++ % 2) ? "work" : "not work";
        size_t len = snprintf(buffer, BUFFER_SIZE, "Device state: %s", state);
        
        // Send device state to server
        if (sock_udp_send(&sock, buffer, len, &remote) < 0) {
            perror("Failed to send UDP message");
        } else {
            printf("Sent: %s\n", buffer);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    // Clean up
    sock_udp_close(&sock);

    return 0;
}