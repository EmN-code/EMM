#include <xtimer.h>    // Required for xtimer_sleep
#include <stdio.h>     // For printf
#include <net/sock/udp.h> // For UDP socket functions
#include <arpa/inet.h> // For inet_pton function to convert IP addresses

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5

void send_device_state(sock_udp_ep_t *remote) {
    char *state_message = "device state: work"; // Example message
    if (sock_udp_send(NULL, state_message, strlen(state_message), remote) < 0) {
        printf("Error sending message\n");
    } else {
        printf("Message sent: %s\n", state_message);
    }
}

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

    // Convert server IP
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        printf("Error: Could not convert server IP\n");
        return -1;
    }

    while (1) {
        send_device_state(&server);
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}