#include <time.h>
#include <stdio.h>
#include <paho_mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_mqtt_client"
#define MQTT_TOPIC "state"
#define PUBLISH_INTERVAL_SEC 5

static MQTTClient client;

void publish_state(MQTTClient client) {
    MQTTMessage message;
    char payload[20];
    int rc;

    sprintf(payload, "work");

    message.payload = payload;
    message.payloadlen = strlen(payload);
    message.qos = QOS0;
    message.retained = 0;

    rc = MQTTPublish(client, MQTT_TOPIC, &message);
    printf("Published %s to topic %s with result code %d\n", payload, MQTT_TOPIC, rc);
}

void mqtt_connect() {
    Network n;
    Timer timer;
    int rc;

    NetworkInit(&n);
    NetworkConnect(&n, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &n, PUBLISH_INTERVAL_SEC * 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
    }
}

int main(void) {
    mqtt_connect();

    while (1) {
        publish_state(client);
        sleep(PUBLISH_INTERVAL_SEC);
    }

    return 0;
}