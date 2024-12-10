
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
static char buf[128];

void send_state(void) {
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    // Toggle state
    state_index = (state_index + 1) % 2;

    // Prepare the message
    snprintf(buf, sizeof(buf), "Device state: %s", states[state_index]);

    // Send the message
    if (sock_udp_send(NULL, buf, strlen(buf), &server) < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void) {
    // Initialize the UDP socket
    if (sock_udp_create(NULL, NULL, &server, 0) < 0) {
        puts("Error initializing UDP socket");
        return 1;
    }

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Send device state every 5 seconds
    while (1) {
        send_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
