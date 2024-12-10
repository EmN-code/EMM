#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"
#define INTERVAL 5000000  // 5 seconds in microseconds
static sock_udp_t sock;
static sock_udp_ep_t local, remote;

static void init_udp_socket(void) {
    // Initialize local endpoint
    local.port = 0;  // Use any available port
    local.family = AF_INET;

    // Initialize remote endpoint
    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr);

    // Create the UDP socket
    sock_udp_create(&sock, &local, &remote, 0);
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
        const char *state = MESSAGE_WORK;

        // Send the device state
        send_device_state(state);

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}