#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"

static sock_udp_t sock;
static sock_udp_ep_t remote;

static void init_udp_socket(void) {
    // Create the UDP socket
    sock_udp_create(&sock, NULL, NULL, 0);

    // Set the remote endpoint
    remote.port = SERVER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr, SERVER_IP);
}

static void send_device_state(const char *state) {
    // Send the message
    sock_udp_send(&sock, state, strlen(state), &remote);
}

int main(void) {
    // Initialize the UDP socket
    init_udp_socket();

    while (1) {
        // Determine the device state (for demonstration, we'll alternate between "work" and "not work")
        static bool is_working = true;
        const char *state = is_working ? MESSAGE_WORK : MESSAGE_NOT_WORK;

        // Send the device state
        send_device_state(state);

        // Toggle the device state
        is_working = !is_working;

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}