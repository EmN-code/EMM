
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void publish_state(sock_tcp_t *sock) {
    static const char message[] = MQTT_MESSAGE;
    ssize_t res = sock_tcp_write(sock, message, strlen(message));
    if (res < 0) {
        puts("Error sending message");
    } else {
        printf("Published: %s\n", message);
    }
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t remote;
    int result;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    result = inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Create a TCP socket
    result = sock_tcp_create(&sock, NULL, &remote, 0);
    if (result < 0) {
        puts("Error creating socket");
        return 1;
    }

    // Connect to the MQTT broker
    result = sock_tcp_connect(&sock, &remote, 0);
    if (result < 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    // Main loop to publish state every 5 seconds
    while (1) {
        publish_state(&sock);
        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
