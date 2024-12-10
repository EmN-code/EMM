
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "net/af.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/mqtt.h"
#include "MQTTClient.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define MQTT_TOPIC          "state"
#define MQTT_MESSAGE        "work or not work"
#define MQTT_CLIENT_ID      "esp32_client"
#define MQTT_KEEPALIVE      60
#define MQTT_QOS            0
#define MQTT_RETAIN         false
#define MQTT_CLEANSESSION   true

static Network network;
static MQTTClient mqttClient;

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
}

static void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEANSESSION;

    int rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("MQTT connect failed with error code %d\n", rc);
        return;
    }

    printf("MQTT connected\n");
}

static void mqtt_disconnect(void)
{
    MQTTDisconnect(&mqttClient);
    printf("MQTT disconnected\n");
}

static void network_init(void)
{
    int rc = sock_tcp_ep_fmt(&network.sock, AF_INET, MQTT_BROKER_IP, MQTT_BROKER_PORT, NULL);
    if (rc < 0) {
        printf("Failed to initialize network: %d\n", rc);
        return;
    }

    rc = sock_tcp_connect(&network.sock, NULL, SOCK_FLAGS_REUSE_EP);
    if (rc < 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return;
    }

    printf("Network initialized and connected to MQTT broker\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    network_init();

    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    mqtt_disconnect();

    return 0;
}
