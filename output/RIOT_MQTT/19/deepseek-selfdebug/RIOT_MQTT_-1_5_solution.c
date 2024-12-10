
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void publish_state(const char *state) {
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "state: %s", state);
    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        puts("Error sending message");
    } else {
        printf("Published: %s\n", buf);
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
    remote.port = MQTT_BROKER_PORT;
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error parsing MQTT broker IP address");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state("work");
        xtimer_usleep(PUBLISH_INTERVAL);
        publish_state("not work");
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Clean up
    sock_udp_close(&sock);
    return 0;
}
