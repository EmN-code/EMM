
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include <net/gnrc/ipv4.h>  // Include the correct header for IPv4

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void publish_state(void) {
    char buf[MQTT_MESSAGE_LEN];
    memcpy(buf, MQTT_MESSAGE, MQTT_MESSAGE_LEN);

    if (sock_udp_send(&sock, buf, MQTT_MESSAGE_LEN, &remote) < 0) {
        puts("Error sending message");
    } else {
        puts("State published");
    }
}

int main(void) {
    // Initialize the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set up the remote endpoint (MQTT broker)
    remote.family = AF_INET;
    remote.port = MQTT_SERVER_PORT;
    if (inet_pton(AF_INET, MQTT_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Close the socket (this part is unreachable in this example)
    sock_udp_close(&sock);

    return 0;
}
