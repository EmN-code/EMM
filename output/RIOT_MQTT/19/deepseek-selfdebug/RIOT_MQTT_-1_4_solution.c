
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/gnrc/ipv4.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void publish_state(void) {
    uint8_t buf[128];
    size_t len = snprintf((char *)buf, sizeof(buf), "{\"state\":\"%s\"}", MQTT_MESSAGE);

    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        puts("Error sending message");
    } else {
        puts("State published");
    }
}

int main(void) {
    // Initialize the UDP socket
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set up the remote endpoint (MQTT broker)
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error parsing MQTT broker IP address");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Close the socket (this will never be reached in this example)
    sock_udp_close(&sock);

    return 0;
}
