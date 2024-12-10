
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_ID 1
#define MQTT_PAYLOADLEN strlen(MQTT_MESSAGE)
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static sock_udp_t mqtt_sock;
static sock_udp_ep_t broker_ep = SOCK_IPV4_EP_ANY;

void mqtt_connect(void) {
    int result;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;

    result = sock_udp_create(&mqtt_sock, &local, &broker_ep, 0);
    if (result < 0) {
        printf("Error creating UDP socket: %d\n", result);
        return;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.willFlag = (MQTT_WILL_TOPIC != NULL);
    options.will.topicName.cstring = MQTT_WILL_TOPIC;
    options.will.message.cstring = MQTT_WILL_MESSAGE;
    options.will.qos = MQTT_WILL_QOS;
    options.will.retained = MQTT_WILL_RETAIN;
    options.username.cstring = MQTT_USERNAME;
    options.password.cstring = MQTT_PASSWORD;

    MQTTClient client;
    MQTTClientInit(&client, NULL, 1000, NULL, 0, NULL, 0);

    int rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Error connecting to MQTT broker: %d\n", rc);
        return;
    }

    printf("Connected to MQTT broker\n");
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = MQTT_DUP;
    message.id = MQTT_ID;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = MQTT_PAYLOADLEN;

    int rc = MQTTPublish(&client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("Error publishing message: %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    printf("Starting MQTT example\n");

    broker_ep.family = AF_INET;
    broker_ep.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_ep.addr.ipv4);

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
