
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/socket.h>
#include <net/conn/tcp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_PAYLOAD_LEN 128
#define MQTT_CLIENT_ID "esp32_client"

#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char mqtt_payload[MQTT_PAYLOAD_LEN];
static int sock;

void mqtt_connect(void) {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        puts("Error creating socket");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MQTT_BROKER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(MQTT_BROKER_IP);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        puts("Error connecting to MQTT broker");
        close(sock);
        return;
    }

    puts("Connected to MQTT broker");
}

void mqtt_publish(const char *topic, const char *payload) {
    uint8_t mqtt_packet[128];
    uint8_t *ptr = mqtt_packet;

    // Fixed header
    *ptr++ = 0x30; // MQTT Control Packet type (PUBLISH) with QoS 0
    *ptr++ = strlen(topic) + strlen(payload) + 2; // Remaining length

    // Variable header
    *ptr++ = (uint8_t)(strlen(topic) >> 8); // Topic length MSB
    *ptr++ = (uint8_t)(strlen(topic) & 0xFF); // Topic length LSB
    memcpy(ptr, topic, strlen(topic));
    ptr += strlen(topic);

    // Payload
    memcpy(ptr, payload, strlen(payload));
    ptr += strlen(payload);

    // Send the packet
    send(sock, mqtt_packet, ptr - mqtt_packet, 0);
}

void publish_state(void) {
    static int state = 0;
    const char *states[] = {"work", "not work"};

    snprintf(mqtt_payload, MQTT_PAYLOAD_LEN, "%s", states[state]);
    mqtt_publish(MQTT_TOPIC, mqtt_payload);

    state = !state; // Toggle state
}

int main(void) {
    puts("Starting MQTT client on ESP32");

    mqtt_connect();

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    close(sock);
    return 0;
}
