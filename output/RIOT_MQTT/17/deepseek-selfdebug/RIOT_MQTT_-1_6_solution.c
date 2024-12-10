
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static sock_tcp_ep_t remote;

void publish_state(void) {
    char message[MQTT_MESSAGE_LEN + 1];
    strncpy(message, MQTT_MESSAGE, MQTT_MESSAGE_LEN);
    message[MQTT_MESSAGE_LEN] = '\0';

    ssize_t res = sock_tcp_send(&sock, message, MQTT_MESSAGE_LEN, 0);
    if (res < 0) {
        printf("Failed to send message: %zd\n", res);
    } else {
        printf("Published: %s\n", message);
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    // Connect to the MQTT broker
    int res = sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        printf("Failed to connect to MQTT broker: %d\n", res);
        return 1;
    }

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
        publish_state();
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
