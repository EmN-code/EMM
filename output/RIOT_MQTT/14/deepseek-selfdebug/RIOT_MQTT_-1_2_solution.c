
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static struct Network network;
static MQTTClient mqttClient;

void mqtt_connection_lost(void *context, char *cause) {
    (void)context;
    (void)cause;
    printf("Connection lost\n");
}

int mqtt_message_arrived(void *context, char *topicName, int topicLen, MQTTMessage *message) {
    (void)context;
    (void)topicName;
    (void)topicLen;
    (void)message;
    return 1;
}

void mqtt_delivery_complete(void *context, MQTTMessageId messageId) {
    (void)context;
    (void)messageId;
}

void connect_to_broker(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.MQTTVersion = 3;

    int rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
    } else {
        printf("Connected to MQTT broker\n");
    }
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int sock = sock_tcp_create(&local, &remote, 0);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    network.sock = sock;
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;

    MQTTClient_init(&mqttClient, &network, 1000, NULL, NULL, NULL);
    mqttClient.connectionLost = mqtt_connection_lost;
    mqttClient.messageArrived = mqtt_message_arrived;
    mqttClient.deliveryComplete = mqtt_delivery_complete;

    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
