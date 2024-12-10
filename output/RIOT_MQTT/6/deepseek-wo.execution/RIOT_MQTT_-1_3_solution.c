
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/sock.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <ztimer.h>
#include <paho_mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEP_ALIVE 60
#define MQTT_QOS 1
#define MQTT_RETAIN 0

typedef struct {
    int socket;
} Network;

void NetworkInit(Network *n) {
    n->socket = -1;
}

int NetworkConnect(Network *n, char *address_ip, int port_number) {
    struct sockaddr_in server_addr;
    n->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (n->socket < 0) {
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    inet_pton(AF_INET, address_ip, &server_addr.sin_addr);

    if (connect(n->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(n->socket);
        return -1;
    }

    return 0;
}

void NetworkDisconnect(Network *n) {
    if (n->socket >= 0) {
        close(n->socket);
        n->socket = -1;
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    char payload[20];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    connect_data.clientID.cstring = MQTT_CLIENT_ID;
    connect_data.keepAliveInterval = MQTT_KEEP_ALIVE;
    connect_data.cleansession = 1;

    if (MQTTConnect(&client, &connect_data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        NetworkDisconnect(&network);
        return 1;
    }

    while (1) {
        message.qos = MQTT_QOS;
        message.retained = MQTT_RETAIN;
        message.dup = 0;
        message.payload = payload;

        snprintf(payload, sizeof(payload), "work");
        message.payloadlen = strlen(payload);
        MQTTPublish(&client, MQTT_TOPIC, &message);

        ztimer_sleep(ZTIMER_MSEC, 5000);

        snprintf(payload, sizeof(payload), "not work");
        message.payloadlen = strlen(payload);
        MQTTPublish(&client, MQTT_TOPIC, &message);

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
