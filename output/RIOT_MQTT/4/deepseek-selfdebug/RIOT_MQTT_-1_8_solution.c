
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void _publish_state(void)
{
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    uint8_t buf[1024];
    size_t len;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid IP address");
        return;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return;
    }

    // Prepare MQTT PUBLISH packet
    // This is a simplified example; actual MQTT packet construction is more complex
    // Here we just send a simple string as payload
    len = snprintf((char *)buf, sizeof(buf), "PUBLISH %s %s", MQTT_TOPIC, MQTT_PAYLOAD);

    // Send the packet
    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        puts("Error: failed to send UDP packet");
    } else {
        printf("Published: %s\n", MQTT_PAYLOAD);
    }

    // Close the socket
    sock_udp_close(&sock);
}

int main(void)
{
    puts("MQTT Publisher starting...");

    while (1) {
        _publish_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
