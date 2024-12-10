#include "thread.h"
#include "xtimer.h"
#include "stdio.h"
#include "mqtt.h"
#include "net/emcute.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STATE_TOPIC "device/state"

static mqtt_client_t mqqt_client;
static Network network;

int publish_state(mqtt_client_t *client) {
    MQTTMessage message;
    char payload[20];
    strcpy(payload, "work");
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);
    return MQTTPublish(client, STATE_TOPIC, &message);
}

int mqtt_connect_and_publish() {
    int rc;
    printf("Connecting to MQTT Broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to the network.\n");
            MQTTClientInit(&mqqt_client, &network, 1000, NULL, 0, NULL, 0);
            while (1) {
                rc = publish_state(&mqqt_client);
                if (rc != 0) {
                    printf("Failed to publish message, trying again...\n");
                }
                xtimer_sleep(5);
            }
        }
    }
    return 0;
}

int main(void) {
    printf("MQTT Client example\n");
    mqtt_connect_and_publish();
    return 0;
}