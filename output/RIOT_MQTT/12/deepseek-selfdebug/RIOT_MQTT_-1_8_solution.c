
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/sock/udp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void publish_state(void) {
    char buf[64];
    int res;

    // Prepare the message
    snprintf(buf, sizeof(buf), "{\"state\":\"%s\"}", MQTT_MESSAGE);

    // Send the message
    res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        puts("Error sending message");
    } else {
        printf("Published: %s\n", buf);
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the UDP socket
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;  // Let the system assign a random port

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set up the remote endpoint
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid address");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep_until(&last_wakeup, INTERVAL);
    }

    return 0;
}
