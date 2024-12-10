#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"
static sock_udp_t sock;
static sock_udp_ep_t server_ep;

static void init_udp_socket(void) {
    // Create the UDP socket
    sock_udp_create(&sock, NULL, NULL, 0);

    // Set the server endpoint
    server_ep.port = SERVER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&server_ep.addr, SERVER_IP);
}
static void send_device_state(const char *state) {
    // Prepare the message
    char message[10];
    strncpy(message, state, sizeof(message));

    // Send the message
    ssize_t res = sock_udp_send(&sock, message, strlen(message), &server_ep);
    if (res < 0) {
        printf("Failed to send message: %d\n", (int)res);
    } else {
        printf("Sent message: %s\n", message);
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