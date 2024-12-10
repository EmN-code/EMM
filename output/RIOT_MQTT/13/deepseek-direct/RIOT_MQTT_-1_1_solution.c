
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/socket.h>
#include <net/conn/tcp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_QOS 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "esp32_client"

#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"

static int mqtt_connect(int *sock) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_aton(MQTT_BROKER_IP, &server_addr.sin_addr);

    *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*sock < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    if (connect(*sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    return 0;
}

static void mqtt_publish(int sock, const char *topic, const char *payload) {
    uint8_t mqtt_packet[128];
    uint8_t *ptr = mqtt_packet;
    uint16_t len = 0;

    // Fixed header
    *ptr++ = 0x30; // MQTT PUBLISH packet
    len = strlen(topic) + strlen(payload) + 2;
    *ptr++ = len;

    // Variable header
    *ptr++ = (uint8_t)(strlen(topic) >> 8);
    *ptr++ = (uint8_t)(strlen(topic) & 0xFF);
    memcpy(ptr, topic, strlen(topic));
    ptr += strlen(topic);

    // Payload
    memcpy(ptr, payload, strlen(payload));

    send(sock, mqtt_packet, ptr - mqtt_packet + strlen(payload), 0);
}

int main(void) {
    int sock;
    bool state = true;

    if (mqtt_connect(&sock) < 0) {
        return -1;
    }

    while (1) {
        const char *payload = state ? PAYLOAD_WORK : PAYLOAD_NOT_WORK;
        mqtt_publish(sock, MQTT_TOPIC, payload);
        printf("Published: %s\n", payload);

        state = !state;
        xtimer_sleep(5);
    }

    close(sock);
    return 0;
}
