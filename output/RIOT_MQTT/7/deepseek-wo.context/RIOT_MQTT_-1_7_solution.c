
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static MQTTClient mqttClient;
static Network network;

void network_init(void) {
    network.mqttread = NULL;
    network.mqttwrite = NULL;
    network.sock = SOCK_ADDR_ANY_NETIF;
}

void mqtt_connect(void) {
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    int result = MQTTConnect(&mqttClient, &data);
    if (result != 0) {
        printf("MQTT connection failed with error code %d\n", result);
    } else {
        printf("MQTT connected successfully\n");
    }
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int result = MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
    if (result != 0) {
        printf("MQTT publish failed with error code %d\n", result);
    } else {
        printf("MQTT message published successfully\n");
    }
}

int main(void) {
    network_init();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
