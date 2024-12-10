
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define MQTT_KEEPALIVE 60
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + 512)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static Network network;
static MQTTClient mqttClient;

void mqtt_publish(MQTTClient *client, const char *topic, const char *message, int len) {
    MQTTMessage mqttMessage;
    mqttMessage.qos = QOS0;
    mqttMessage.retained = 0;
    mqttMessage.dup = 0;
    mqttMessage.payload = (void *)message;
    mqttMessage.payloadlen = len;

    MQTTPublish(client, topic, &mqttMessage);
}

void mqtt_connect(MQTTClient *client, Network *network, const char *client_id) {
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = (char *)client_id;
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = 1;

    MQTTConnect(client, &data);
}

void mqtt_thread(void *arg) {
    (void)arg;

    int result = NetworkConnect(&network, (char *)MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (result != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);
    mqtt_connect(&mqttClient, &network, MQTT_CLIENT_ID);

    while (1) {
        mqtt_publish(&mqttClient, MQTT_TOPIC, MQTT_MESSAGE, MQTT_MESSAGE_LEN);
        xtimer_sleep(5);
    }
}

int main(void) {
    thread_create(NULL, MQTT_THREAD_STACKSIZE, MQTT_THREAD_PRIORITY, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
