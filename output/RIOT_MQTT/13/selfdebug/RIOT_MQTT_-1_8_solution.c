#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/inet.h>  // For inet_pton
#include <xtimer.h>    // For xtimer_sleep
#include <net/sock/tcp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"

static const char *state_msg = "work";

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t remote;
    int res;

    // Convert IP address from text to binary form
    res = inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        printf("inet_pton() failed\n");
        return -1;
    }
    remote.port = MQTT_BROKER_PORT;

    // Create socket and connect to the MQTT broker
    res = sock_tcp_connect(&sock, &remote, sizeof(remote), 0);
    if (res < 0) {
        printf("Connection to broker failed\n");
        return -1;
    }

    while (1) {
        // Publish message
        res = sock_tcp_send(&sock, state_msg, strlen(state_msg), 0);
        if (res < 0) {
            printf("Failed to send message\n");
            sock_tcp_disconnect(&sock);
            return -1;
        }

        printf("Message sent: %s\n", state_msg);

        // Wait for 5 seconds before sending the next message
        xtimer_sleep(5);
    }

    // Disconnect and clean up
    sock_tcp_disconnect(&sock);
    return 0;
}