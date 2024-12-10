#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"
#define MESSAGE_INTERVAL (5 * 1000000) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

static void init_udp_socket(void) {
    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    remote.addr = inet_addr(SERVER_IP);

    // Create the UDP socket
    sock_udp_create(&sock, NULL, &remote, 0);
}

static void send_device_state(const char *state) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%s", state);

    // Send the message
    ssize_t res = sock_udp_send(&sock, buffer, strlen(buffer), &remote);
    if (res < 0) {
        printf("Failed to send message: %d\n", (int)res);
    } else {
        printf("Sent message: %s\n", buffer);
    }
}

int main(void) {
    // Initialize the UDP socket
    init_udp_socket();

    while (1) {
        // Simulate device state (for demonstration, always send "work")
        send_device_state(MESSAGE_WORK);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}