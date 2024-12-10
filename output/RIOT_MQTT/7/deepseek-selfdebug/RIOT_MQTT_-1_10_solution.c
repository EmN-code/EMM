
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUF_SIZE 1024
#define INTERVAL 5000000  // 5 seconds in microseconds

static char tx_buf[BUF_SIZE];
static sock_tcp_t sock;

void publish_state(void) {
    static int state = 0;
    const char *states[] = {"work", "not work"};

    // Create the MQTT PUBLISH message
    snprintf(tx_buf, BUF_SIZE, "PUBLISH\nTopic: state\nPayload: %s\n\n", states[state]);
    state = !state;  // Toggle state

    // Send the message
    if (sock_tcp_send(&sock, tx_buf, strlen(tx_buf)) < 0) {
        puts("Failed to send data");
    } else {
        puts("State published");
    }
}

int main(void) {
    ipv4_addr_t server_addr;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr) <= 0) {
        puts("Invalid address");
        return 1;
    }
    remote.addr.ipv4 = server_addr;

    // Create a TCP socket
    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        puts("Failed to create socket");
        return 1;
    }

    // Connect to the MQTT broker
    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Failed to connect to server");
        sock_tcp_close(&sock);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);
    return 0;
}
