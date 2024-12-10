#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define MESSAGE_LENGTH 4
#define INTERVAL 5000000  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local, remote;

static void init_udp_socket(void) {
    // Initialize local endpoint
    local.port = 0;  // Use any available port
    local.addr = SOCK_IPV4_ADDR_ANY;  // Bind to any available IPv4 address

    // Initialize remote endpoint
    remote.port = SERVER_PORT;
    remote.addr = ipv4_addr_from_str(SERVER_IP);

    // Create the UDP socket
    sock_udp_create(&sock, &local, &remote, 0);
}

static void send_udp_message(const char *message, size_t length) {
    ssize_t res = sock_udp_send(&sock, message, length, &remote);
    if (res < 0) {
        printf("Failed to send UDP message: %d\n", (int)res);
    } else {
        printf("Sent UDP message: %s\n", message);
    }
}

int main(void) {
    // Initialize the UDP socket
    init_udp_socket();

    while (1) {
        // Send the message
        send_udp_message(MESSAGE, MESSAGE_LENGTH);

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}