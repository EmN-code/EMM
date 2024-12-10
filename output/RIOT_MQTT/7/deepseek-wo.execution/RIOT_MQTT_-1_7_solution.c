
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <xtimer.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <dht.h>
#include <dht_params.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <timex.h>
#include <architecture.h>
#include <stddef.h>
#include <net/emcute.h>
#include <paho_mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 30000
#define MQTT_SEND_BUF_SIZE 100
#define MQTT_READ_BUF_SIZE 100

typedef struct {
    int socket;
    struct sockaddr_in addr;
} Network;

void NetworkInit(Network *n) {
    n->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (n->socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }
}

int NetworkConnect(Network *n, char *address_ip, int port_number) {
    n->addr.sin_family = AF_INET;
    n->addr.sin_port = htons(port_number);
    if (inet_pton(AF_INET, address_ip, &n->addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }
    if (connect(n->socket, (struct sockaddr *)&n->addr, sizeof(n->addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }
    return 0;
}

void NetworkDisconnect(Network *n) {
    close(n->socket);
}

typedef struct {
    Network *network;
    unsigned int command_timeout_ms;
    unsigned char *sendbuf;
    size_t sendbuf_size;
    unsigned char *readbuf;
    size_t readbuf_size;
} MQTTClient;

void MQTTClientInit(MQTTClient *client, Network *network, unsigned int command_timeout_ms,
                    unsigned char *sendbuf, size_t sendbuf_size, unsigned char *readbuf, size_t readbuf_size) {
    client->network = network;
    client->command_timeout_ms = command_timeout_ms;
    client->sendbuf = sendbuf;
    client->sendbuf_size = sendbuf_size;
    client->readbuf = readbuf;
    client->readbuf_size = readbuf_size;
}

int MQTTConnect(MQTTClient *client, MQTTPacket_connectData *options) {
    // Implement MQTT connect logic here
    return 0;
}

int MQTTPublish(MQTTClient *client, const char *topic, MQTTMessage *message) {
    // Implement MQTT publish logic here
    return 0;
}

int MQTTDisconnect(MQTTClient *client) {
    // Implement MQTT disconnect logic here
    return 0;
}

int main() {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[MQTT_SEND_BUF_SIZE];
    unsigned char readbuf[MQTT_READ_BUF_SIZE];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    NetworkInit(&network);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        perror("Failed to connect to MQTT broker");
        return EXIT_FAILURE;
    }

    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        perror("Failed to connect to MQTT broker");
        NetworkDisconnect(&network);
        return EXIT_FAILURE;
    }

    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            perror("Failed to publish message");
        }
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return EXIT_SUCCESS;
}
