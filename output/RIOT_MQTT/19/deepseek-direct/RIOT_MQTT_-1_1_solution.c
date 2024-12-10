
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/socket.h>
#include <net/conn/tcp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "esp32_client"

#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static int mqtt_connect(int sock) {
    uint8_t connect_packet[] = {
        0x10, 0x26, // Fixed header
        0x00, 0x04, 'M', 'Q', 'T', 'T', 0x04, 0x02, // Protocol name and level
        0x00, 0x3C, // Keepalive
        0x00, 0x0B, 'e', 's', 'p', '3', '2', '_', 'c', 'l', 'i', 'e', 'n', 't' // Client ID
    };

    return send(sock, connect_packet, sizeof(connect_packet), 0);
}

static int mqtt_publish(int sock, const char *topic, const char *payload) {
    uint8_t publish_packet[128];
    uint16_t topic_len = strlen(topic);
    uint16_t payload_len = strlen(payload);
    uint16_t packet_len = 2 + topic_len + payload_len;

    publish_packet[0] = 0x30; // Fixed header (PUBLISH)
    publish_packet[1] = packet_len;
    publish_packet[2] = (topic_len >> 8) & 0xFF;
    publish_packet[3] = topic_len & 0xFF;
    memcpy(&publish_packet[4], topic, topic_len);
    memcpy(&publish_packet[4 + topic_len], payload, payload_len);

    return send(sock, publish_packet, 4 + topic_len + payload_len, 0);
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error initializing network stack");
        return 1;
    }

    // Create a TCP socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        puts("Error creating socket");
        return 1;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_aton(MQTT_BROKER_IP, &server_addr.sin_addr);

    // Connect to the MQTT broker
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        puts("Error connecting to MQTT broker");
        close(sock);
        return 1;
    }

    // Send MQTT CONNECT packet
    if (mqtt_connect(sock) < 0) {
        puts("Error sending MQTT CONNECT packet");
        close(sock);
        return 1;
    }

    // Main loop to publish state every 5 seconds
    bool work_state = true;
    while (1) {
        const char *payload = work_state ? MQTT_PAYLOAD_WORK : MQTT_PAYLOAD_NOT_WORK;
        if (mqtt_publish(sock, MQTT_TOPIC, payload) < 0) {
            puts("Error publishing MQTT message");
            close(sock);
            return 1;
        }

        printf("Published: %s\n", payload);
        work_state = !work_state; // Toggle state

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    close(sock);
    return 0;
}
