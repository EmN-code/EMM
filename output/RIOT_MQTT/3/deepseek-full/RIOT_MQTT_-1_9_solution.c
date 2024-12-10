
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <timex.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "riot_esp32"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 1
#define MQTT_RETAIN 0
#define MQTT_PAYLOAD_LEN 128
#define MQTT_BUF_SIZE 1024
#define MQTT_CMD_TIMEOUT 2000

typedef struct {
    int socket;
    struct sockaddr_in server_addr;
} Network;

typedef struct {
    Network network;
    unsigned char sendbuf[MQTT_BUF_SIZE];
    unsigned char readbuf[MQTT_BUF_SIZE];
} MQTTClient;

void NetworkInit(Network *n) {
    n->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (n->socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }
}

int NetworkConnect(Network *n, char *address_ip, int port_number) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    inet_pton(AF_INET, address_ip, &server_addr.sin_addr);

    if (connect(n->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect to server");
        return -1;
    }
    return 0;
}

void MQTTClientInit(MQTTClient *client, Network *network) {
    client->network = *network;
}

int MQTTConnect(MQTTClient *client, MQTTPacket_connectData *options) {
    MQTTPacket_connectData default_options = MQTTPacket_connectData_initializer;
    default_options.clientID.cstring = MQTT_CLIENT_ID;
    default_options.keepAliveInterval = MQTT_KEEPALIVE;
    default_options.cleansession = 1;
    default_options.MQTTVersion = 4;

    int rc = MQTTConnectWithResults(client, &default_options, NULL);
    if (rc != 0) {
        perror("Failed to connect to MQTT broker");
        return -1;
    }
    return 0;
}

int MQTTPublish(MQTTClient *client, const char *topic, const char *payload) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(client, topic, &message);
    if (rc != 0) {
        perror("Failed to publish message");
        return -1;
    }
    return 0;
}

int main(void) {
    Network network;
    MQTTClient client;

    NetworkInit(&network);
    if (NetworkConnect(&network, EMQX_SERVER_IP, EMQX_SERVER_PORT) < 0) {
        return EXIT_FAILURE;
    }

    MQTTClientInit(&client, &network);
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    if (MQTTConnect(&client, &options) < 0) {
        return EXIT_FAILURE;
    }

    while (1) {
        const char *state = "work";
        if (MQTTPublish(&client, MQTT_TOPIC, state) < 0) {
            return EXIT_FAILURE;
        }
        xtimer_sleep(5);
    }

    return EXIT_SUCCESS;
}
