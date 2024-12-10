#include <stdliob.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

int main(void) {
    printf("ESP32 MQTT client example\n");

    sock_udp_ep_t server = {
        .family = AF_INET,
        .netif  = SOCK_ADDR_ANY_NETIF,
        .port   = MQTT_BROKER_PORT
    };

    // Adding socket address initialization
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &server.addr.ipv4) != 1) {
        puts("Error: Invalid server address\n");
        return 1;
    }

    // Create the UDP socket
    sock_udp_t sock;
    uint8_t buf[128];

    if (sock_udp_create(&sock, &server, NULL, 0) < 0) {
        puts("Error: Unable to create UDP socket\n");
        return 1;
    }

    int count = 0;
    while (1) {
        // Set up MQTT publish packet manually
        memset(buf, 0, sizeof(buf));
        size_t message_len = snprintf((char *)buf, sizeof(buf), "Publish number: %d", count++);

        if (sock_udp_send(&sock, buf, message_len, &server) < 0) {
            puts("Error: Failed to send packet\n");
        }

        printf("Sent packet: %s\n", buf);

        // Delay between publishes
        busy_wait_ms(PUBLISH_INTERVAL * 1000); // Placeholder: Implement 'busy_wait_ms' function
    }

    return 0;
}

// Implementation of busy_wait_ms function
void busy_wait_ms(unsigned ms) {
    volatile unsigned int count = 0;
    while (count < ms * 1000) {
        count++;
    }
}