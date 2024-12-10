
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MSG_SIZE 128
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char msg_buf[MSG_SIZE];

static void publish_state(int sock) {
    const char *state = (rand() % 2 == 0) ? "work" : "not work";
    snprintf(msg_buf, sizeof(msg_buf), "state: %s", state);
    sock_tcp_send(sock, msg_buf, strlen(msg_buf));
}

int main(void) {
    sock_tcp_t sock;
    struct sock_tcp_ep ep = { .family = AF_INET };
    struct in_addr server_addr;

    // Convert IP address from string to binary form
    inet_pton(AF_INET, SERVER_IP, &server_addr);
    ep.addr.ipv4 = server_addr;
    ep.port = SERVER_PORT;

    // Create a TCP socket
    if (sock_tcp_create(&sock, NULL, &ep, 0) < 0) {
        puts("Error creating socket");
        return 1;
    }

    // Connect to the MQTT broker
    if (sock_tcp_connect(&sock, &ep, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error connecting to server");
        sock_tcp_close(&sock);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state(sock);
        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);
    return 0;
}
