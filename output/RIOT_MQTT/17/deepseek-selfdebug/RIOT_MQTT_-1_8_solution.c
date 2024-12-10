
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static sock_tcp_ep_t remote;

int main(void)
{
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    // Initialize the socket
    sock_tcp_create(&sock, NULL, &remote, 0);

    // Connect to the MQTT broker
    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: Could not connect to MQTT broker");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        // Send the MQTT message
        if (sock_tcp_send(&sock, MQTT_MESSAGE, MQTT_MESSAGE_LEN, 0) < 0) {
            puts("Error: Could not send message to MQTT broker");
            break;
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL_MS);
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
