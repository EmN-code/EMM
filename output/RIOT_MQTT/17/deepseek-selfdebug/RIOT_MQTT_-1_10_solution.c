
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/sock/tcp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_ep_t remote;
    char buf[100];
    int res;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    // Create a TCP socket
    res = sock_tcp_create(&sock, NULL, &remote, 0);
    if (res < 0) {
        printf("Failed to create socket: %d\n", res);
        return 1;
    }

    // Connect to the MQTT broker
    res = sock_tcp_connect(&sock, &remote, 0);
    if (res < 0) {
        printf("Failed to connect to MQTT broker: %d\n", res);
        sock_tcp_close(&sock);
        return 1;
    }

    // Main loop to publish state every 5 seconds
    while (1) {
        // Prepare the MQTT message
        snprintf(buf, sizeof(buf), "PUBLISH %s %s\n", MQTT_TOPIC, MQTT_MESSAGE);

        // Send the message
        res = sock_tcp_send(&sock, buf, strlen(buf), 0);
        if (res < 0) {
            printf("Failed to send message: %d\n", res);
            sock_tcp_close(&sock);
            return 1;
        }

        // Wait for 5 seconds before sending the next message
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
