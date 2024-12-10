#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/emcute.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "riot-client"
#define PUB_TOPIC "device/state"

static int _setup_mqtt(MQTTClient* client) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = CLIENT_ID;
    options.keepAliveInterval = 10;
    options.cleansession = 1;

    int rc;

    while (1) {
        rc = MQTTConnect(client, &options);
        if (rc != 0) {
            printf("MQTT connection failed\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("MQTT connected\n");
            break;
        }
    }

    return rc;
}

static int _publish_state(MQTTClient* client, char* state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*) state;
    message.payloadlen = strlen(state);

    int rc = MQTTPublish(client, PUB_TOPIC, &message);
    if (rc != 0) {
        printf("Error while publishing data\n");
    }
    return rc;
}

int main(void) {
    MQTTClient client;
    Network network;

    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Network connected\n");
            break;
        }
    }

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    _setup_mqtt(&client);

    while (1) {
        _publish_state(&client, "work");
        xtimer_sleep(5);
    }

    return 0;
}