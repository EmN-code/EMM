
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/af.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t remote;

void publish_state(void) {
    char buf[64];
    int res;

    // Prepare the MQTT PUBLISH packet
    snprintf(buf, sizeof(buf), "PUB %s %s\n", TOPIC, PAYLOAD);

    // Send the packet to the broker
    res = sock_udp_send(&sock, buf, strlen(buf), &remote);
    if (res < 0) {
        puts("Error sending message");
    } else {
        printf("Published: %s\n", buf);
    }
}

int main(void) {
    int res;

    // Initialize the UDP socket
    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set up the remote endpoint (MQTT broker)
    remote.family = AF_INET;
    remote.port = BROKER_PORT;
    res = inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Error parsing broker IP address");
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
