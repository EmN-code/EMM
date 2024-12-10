#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds
#define CLIENT_ID "esp32_mqtt_client"
#define TOPIC "device/state"

Network network;
MQTTClient client;
unsigned char sendbuf[100], readbuf[100];

void connect_to_broker(void) {
    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
    } else {
        printf("Connected to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    }
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen("work");

    if (MQTTPublish(&client, TOPIC, &message) != SUCCESS) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published: work\n");
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}