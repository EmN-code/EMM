#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/mqtt.h"
#include "xtimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define MQTT_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

// Network structure
struct Network {
    sock_udp_t sock;
    int (*mqttread)(struct Network*, unsigned char*, int, int);
    int (*mqttwrite)(struct Network*, unsigned char*, int, int);
};

// MQTT client structure
struct MQTTClient {
    struct Network* network;
    unsigned int command_timeout_ms;
    unsigned char* sendbuf;
    size_t sendbuf_size;
    unsigned char* readbuf;
    size_t readbuf_size;
};

// Function prototypes
void network_init(struct Network* n);
int network_connect(struct Network* n, const char* address_ip, int port_number);
void network_disconnect(struct Network* n);
int mqtt_connect(struct MQTTClient* client);
int mqtt_publish(struct MQTTClient* client, const char* topic, const char* message);
void mqtt_disconnect(struct MQTTClient* client);

// Network functions
void network_init(struct Network* n) {
    n->sock = SOCK_UDP_CREATE(NULL, NULL, 0);
    n->mqttread = (int (*)(struct Network*, unsigned char*, int, int))sock_udp_recv;
    n->mqttwrite = (int (*)(struct Network*, unsigned char*, int, int))sock_udp_send;
}

int network_connect(struct Network* n, const char* address_ip, int port_number) {
    sock_udp_ep_t remote = { .port = port_number };
    inet_pton(AF_INET, address_ip, &remote.addr.ipv4);
    return sock_udp_connect(n->sock, &remote, 0);
}

void network_disconnect(struct Network* n) {
    sock_udp_close(n->sock);
}

// MQTT functions
int mqtt_connect(struct MQTTClient* client) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    return MQTTConnect(client, &options);
}

int mqtt_publish(struct MQTTClient* client, const char* topic, const char* message) {
    MQTTMessage msg = { .qos = QOS0, .retained = 0, .payload = (void*)message, .payloadlen = strlen(message) };
    return MQTTPublish(client, topic, &msg);
}

void mqtt_disconnect(struct MQTTClient* client) {
    MQTTDisconnect(client);
}

int main(void) {
    struct Network network;
    struct MQTTClient client;

    // Initialize network
    network_init(&network);

    // Connect to MQTT broker
    if (network_connect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Initialize MQTT client
    client.network = &network;
    client.command_timeout_ms = 30000;
    client.sendbuf = malloc(1024);
    client.readbuf = malloc(1024);
    client.sendbuf_size = 1024;
    client.readbuf_size = 1024;

    // Connect to MQTT broker
    if (mqtt_connect(&client) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        // Publish state
        const char* message = (rand() % 2 == 0) ? MQTT_MESSAGE_WORK : MQTT_MESSAGE_NOT_WORK;
        if (mqtt_publish(&client, MQTT_TOPIC, message) != SUCCESS) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", message);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    // Disconnect from MQTT broker
    mqtt_disconnect(&client);

    // Free allocated buffers
    free(client.sendbuf);
    free(client.readbuf);

    // Disconnect network
    network_disconnect(&network);

    return 0;
}