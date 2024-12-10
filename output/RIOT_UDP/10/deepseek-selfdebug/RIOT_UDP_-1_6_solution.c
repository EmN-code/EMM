
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // Include this for inet_pton
#include "xtimer.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t server = SOCK_IPV4_EP_ANY;

void send_state(const char *state) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%s", state);
    ssize_t res = sock_udp_send(&sock, buf, strlen(buf), &server);
    if (res < 0) {
        puts("Error sending UDP message");
    } else {
        printf("Sent: %s\n", buf);
    }
}

int main(void) {
    // Initialize the UDP socket
    sock_udp_create(&sock, NULL, NULL, 0);

    // Set up the server endpoint
    server.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    while (1) {
        send_state("work");
        xtimer_usleep(INTERVAL);
        send_state("not work");
        xtimer_usleep(INTERVAL);
    }

    // Clean up
    sock_udp_close(&sock);
    return 0;
}
