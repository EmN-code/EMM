#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/mqttsn.h>
#include "MQTTClient.h"  // This is ensured to be the last included header file

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5 // seconds

static void mqtt_publish_state(MQTTClient* client) {
    char* topic = "device/state";
    char* message = "work or not work";
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void*)message;
    mqtt_message.payloadlen = strlen(message);
    int rc;

    printf("Publishing message: %s\n", message);
    if ((rc = MQTTPublish(client, topic, &mqtt_message)) != 0) {
        printf("Error publishing message: %d\n", rc);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    int rc;

    printf("Initializing network\n");
    NetworkInit(&network);

    while (1) {
        printf("Connecting to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to MQTT broker\n");
        }

        MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

        MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
        connectData.MQTTVersion = 3;
        connectData.clientID.cstring = "RIOT_ESP32_Client";

        while ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
            printf("MQTT connection failed, retrying...\n");
            xtimer_sleep(1);
        }
        printf("MQTT connected successfully\n");

        while(1) {  
            mqtt_publish_state(&client);
            xtimer_sleep(PUBLISH_INTERVAL);
        }

        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }

    return 0;
}