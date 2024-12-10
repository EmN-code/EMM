#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"
#include "stdio.h"
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"

static sock_udp_t sock;
static sock_udp_ep_t remote;
static void init_udp_socket(void) {
    // Create a UDP socket
    sock_udp_create(&sock, NULL, NULL, 0);

    // Set the remote endpoint (server)
    ipv4_addr_t server_ip;
    ipv4_addr_from_str(&server_ip, SERVER_IP);
    remote.port = SERVER_PORT;
    remote.addr.ipv4 = server_ip;
}
static void send_device_state(const char *state) {
    // Send the message to the server
    ssize_t res = sock_udp_send(&sock, state, strlen(state), &remote);
    if (res < 0) {
        printf("Failed to send message: %s\n", strerror(-res));
    } else {
        printf("Sent message: %s\n", state);
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
        xtimer_sleep(5);
    }

    return 0;
}