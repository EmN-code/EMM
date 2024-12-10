
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

void publish_state(void) {
    char buf[64];
    int res;

    // Prepare the MQTT PUBLISH packet
    uint8_t mqtt_publish[] = {
        0x30,  // Fixed header (PUBLISH, QoS 0, no retain)
        0x0C,  // Remaining length (12 bytes)
        0x00, 0x05, 's', 't', 'a', 't', 'e',  // Topic length (5) and topic ("state")
        0x00, 0x04, 'w', 'o', 'r', 'k'  // Payload length (4) and payload ("work")
    };

    // Send the MQTT PUBLISH packet
    res = sock_udp_send(&sock, mqtt_publish, sizeof(mqtt_publish), &remote);
    if (res < 0) {
        printf("Failed to send MQTT PUBLISH packet: %d\n", res);
    } else {
        printf("Published state: %s\n", MQTT_PAYLOAD);
    }
}

int main(void) {
    int res;

    // Initialize the remote endpoint for the MQTT broker
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    res = inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Invalid MQTT broker IP address\n");
        return 1;
    }

    // Create a UDP socket
    res = sock_udp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        printf("Failed to create UDP socket: %d\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}
