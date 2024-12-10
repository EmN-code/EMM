
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static sock_tcp_ep_t remote;

void publish_state(void) {
    char message[64];
    snprintf(message, sizeof(message), "PUBLISH %s %s\n", MQTT_TOPIC, MQTT_MESSAGE);

    ssize_t res = sock_tcp_send(&sock, message, strlen(message), &remote);
    if (res < 0) {
        printf("Failed to send message: %d\n", (int)res);
    } else {
        printf("Message sent: %s\n", message);
    }
}

int main(void) {
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    // Create a TCP socket
    int res = sock_tcp_create(&sock, NULL, &remote, 0);
    if (res < 0) {
        printf("Failed to create socket: %d\n", res);
        return 1;
    }

    // Connect to the MQTT broker
    res = sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        printf("Failed to connect to MQTT broker: %d\n", res);
        sock_tcp_close(&sock);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);
    return 0;
}
