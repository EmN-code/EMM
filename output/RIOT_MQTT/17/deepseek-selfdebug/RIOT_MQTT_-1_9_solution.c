
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;

int main(void)
{
    struct sockaddr_in remote;
    int result;

    // Initialize the remote address structure
    memset(&remote, 0, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons(MQTT_BROKER_PORT);

    // Convert the IP address from string to binary form
    result = inet_pton(AF_INET, MQTT_BROKER_IP, &remote.sin_addr);
    if (result != 1) {
        puts("Error: Invalid IP address");
        return 1;
    }

    // Create a TCP socket
    result = sock_tcp_create(&sock, NULL, (struct sockaddr *)&remote, SOCK_ADDR_ANY_NETIF);
    if (result < 0) {
        puts("Error: Failed to create socket");
        return 1;
    }

    // Connect to the MQTT broker
    result = sock_tcp_connect(&sock, (struct sockaddr *)&remote, SOCK_ADDR_ANY_NETIF, 0);
    if (result < 0) {
        puts("Error: Failed to connect to MQTT broker");
        sock_tcp_close(&sock);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        // Send the MQTT message
        result = sock_tcp_send(&sock, MQTT_MESSAGE, MQTT_MESSAGE_LEN, 0);
        if (result < 0) {
            puts("Error: Failed to send message");
            sock_tcp_close(&sock);
            return 1;
        }

        // Wait for 5 seconds
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
