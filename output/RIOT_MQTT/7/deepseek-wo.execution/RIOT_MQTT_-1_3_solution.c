
#include <net/emcute.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

static int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return recv(n->sock, buffer, len, 0);
}

static int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return send(n->sock, buffer, len, 0);
}

static void connect_to_broker(void)
{
    struct sockaddr_in broker_addr;
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_IP, &broker_addr.sin_addr);

    network.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (network.sock < 0) {
        puts("Failed to create socket");
        return;
    }

    if (connect(network.sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        puts("Failed to connect to broker");
        return;
    }

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_ESP32";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if (MQTTConnect(&mqttClient, &data) != 0) {
        puts("Failed to connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    if (MQTTPublish(&mqttClient, TOPIC, &message) != 0) {
        puts("Failed to publish message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
