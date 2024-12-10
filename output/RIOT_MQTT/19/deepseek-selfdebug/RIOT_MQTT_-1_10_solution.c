
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static void mqtt_connect(void)
{
    struct Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.username.cstring = MQTT_USERNAME;
    options.password.cstring = MQTT_PASSWORD;
    options.willFlag = 0;
    options.will.topicName.cstring = MQTT_WILL_TOPIC;
    options.will.message.cstring = MQTT_WILL_MESSAGE;
    options.will.qos = MQTT_WILL_QOS;
    options.will.retained = MQTT_WILL_RETAIN;

    network.mqttread = sock_udp_recv;
    network.mqttwrite = sock_udp_send;

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    int rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("MQTT connect failed with return code %d\n", rc);
        return;
    }

    printf("MQTT connected\n");

    while (1) {
        MQTTMessage message;
        message.qos = MQTT_QOS;
        message.retained = MQTT_RETAIN;
        message.dup = 0;
        message.payload = (void*)MQTT_MESSAGE;
        message.payloadlen = strlen(MQTT_MESSAGE);

        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("MQTT publish failed with return code %d\n", rc);
        } else {
            printf("MQTT message published\n");
        }

        xtimer_sleep(5);
    }
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT, .addr = { .ipv4 = { 47, 102, 103, 1 } } };

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_connect();

    return 0;
}
