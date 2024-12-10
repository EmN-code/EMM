#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define MESSAGE_LENGTH (sizeof(MESSAGE) - 1)
#define INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t server_ep;

static void init_udp_socket() {
    // Create a UDP socket
    sock_udp_create(&sock, NULL, NULL, 0);

    // Set up the server endpoint
    server_ep.port = SERVER_PORT;
    server_ep.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_ep.addr.ipv4);
}

static void send_device_state() {
    char buffer[MESSAGE_LENGTH + 1];
    strcpy(buffer, MESSAGE);

    // Send the message to the server
    ssize_t sent_bytes = sock_udp_send(&sock, buffer, MESSAGE_LENGTH, &server_ep);
    if (sent_bytes < 0) {
        printf("Failed to send message: %d\n", (int)sent_bytes);
    } else {
        printf("Sent message: %s\n", buffer);
    }
}

int main(void) {
    // Initialize the UDP socket
    init_udp_socket();

    while (1) {
        // Send the device state
        send_device_state();

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}