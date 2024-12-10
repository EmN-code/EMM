#include <ztimer.h>
#include <timex.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/af.h>
#include <net/gcoap.h>
#include <dht_params.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_NOT_WORK "Not Work"
#define MQTT_MESSAGE_WORK "Work"

int main(void) {
    int rc;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    // Convert the broker IP address from text to binary form
    struct in_addr broker_ip;
    if (inet_pton(AF_INET, BROKER_IP, &broker_ip) != 1) {
        printf("Error converting broker IP address\n");
        return -1;
    }

    // Initialize network
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // MQTT connection options
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "ESP32Client";

    // Connect to MQTT broker
    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect, return code: %d\n", rc);
        return -1;
    }
    printf("Connected to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    while (1) {
        MQTTMessage message;
        char* payload;

        if (/* some condition to determine work state */) {
            payload = MQTT_MESSAGE_WORK;
        } else {
            payload = MQTT_MESSAGE_NOT_WORK;
        }

        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message, return code: %d\n", rc);
        } else {
            printf("Message published: %s\n", payload);
        }

        ztimer_sleep(ZTIMER_SEC, 5);
    }

    MQTTDisconnect(&client);
    return 0;
}