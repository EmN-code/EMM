
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/inet_pton.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

typedef struct {
    int (*mqttread)(void *context, unsigned char *read_buffer, int len);
    int (*mqttwrite)(void *context, unsigned char *send_buffer, int len);
} Network;

typedef struct {
    Network network;
    sock_tcp_t sock;
} MQTTClient;

int mqtt_read(void *context, unsigned char *read_buffer, int len) {
    MQTTClient *client = (MQTTClient *)context;
    return sock_tcp_read(&client->sock, read_buffer, len, SOCK_NO_TIMEOUT);
}

int mqtt_write(void *context, unsigned char *send_buffer, int len) {
    MQTTClient *client = (MQTTClient *)context;
    return sock_tcp_write(&client->sock, send_buffer, len);
}

void mqtt_connect(MQTTClient *client) {
    sock_tcp_ep_t remote = { .family = AF_INET };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);
    remote.port = MQTT_BROKER_PORT;

    if (sock_tcp_connect(&client->sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: Could not connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void mqtt_publish(MQTTClient *client, const char *topic, const char *payload, int payload_len) {
    unsigned char buf[128];
    int offset = 0;

    // Fixed header
    buf[offset++] = 0x30;  // MQTT Control Packet Type (PUBLISH)
    buf[offset++] = 2 + strlen(topic) + payload_len;  // Remaining length

    // Variable header
    buf[offset++] = (strlen(topic) >> 8) & 0xFF;  // Topic length MSB
    buf[offset++] = strlen(topic) & 0xFF;  // Topic length LSB
    memcpy(&buf[offset], topic, strlen(topic));  // Topic
    offset += strlen(topic);

    // Payload
    memcpy(&buf[offset], payload, payload_len);
    offset += payload_len;

    client->network.mqttwrite(client, buf, offset);
}

int main(void) {
    MQTTClient client;
    client.network.mqttread = mqtt_read;
    client.network.mqttwrite = mqtt_write;

    sock_tcp_init(&client.sock);
    mqtt_connect(&client);

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        mqtt_publish(&client, MQTT_TOPIC, MQTT_PAYLOAD, MQTT_PAYLOAD_LEN);
        puts("Published state");
    }

    return 0;
}
