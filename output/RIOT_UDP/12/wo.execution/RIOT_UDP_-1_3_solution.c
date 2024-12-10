#include "arpa/inet.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/af.h"
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001

#define INTERVAL_USEC (5U * 1000000U) // 5 seconds

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t state = 0;
    char message[20];

    // Convert string IP address to numerical format
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        return 1; // Error: failed to convert IP address
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) != 0) {
        return 1; // Error: failed to create UDP socket
    }

    // Infinite loop to send UDP message every 5 seconds
    while (1) {
        state = !state;
        snprintf(message, sizeof(message), "Device state: %s", state ? "work" : "not work");

        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            // Error: failed to send UDP message
        }

        xtimer_usleep(INTERVAL_USEC);
    }

    return 0;
}