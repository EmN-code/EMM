
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;

static int connect_to_broker(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        printf("Error creating TCP socket\n");
        return -1;
    }

    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        printf("Error connecting to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    return 0;
}

static void publish_state(void) {
    char mqtt_publish_msg[] = {
        0x30,  // Fixed header (PUBLISH, QoS 0, no retain)
        0x0A,  // Remaining length (10 bytes)
        0x00, 0x05, 's', 't', 'a', 't', 'e',  // Topic length (5) and topic ("state")
        'w', 'o', 'r', 'k'  // Payload ("work")
    };

    if (sock_tcp_write(&sock, mqtt_publish_msg, sizeof(mqtt_publish_msg)) < 0) {
        printf("Error sending MQTT message\n");
    } else {
        printf("Published state: %s\n", MQTT_MESSAGE);
    }
}

int main(void) {
    if (connect_to_broker() < 0) {
        return -1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL_MS);
    }

    sock_tcp_close(&sock);
    return 0;
}
