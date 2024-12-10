#include "stdio.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "stdlib.h"

#define MQTT_BROKER_IP       "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define NODE_NAME            "RIOTNode"
#define MQTT_TOPIC           "state"
#define MSG_BUFFER_SIZE      128

static char mqtt_send_buf[128];
static char mqtt_read_buf[128];

void publish_state(MQTTClient* client, const char* state) {
    MQTTMessage message;
    message.payload = (void*) state;
    message.payloadlen = strlen(state);
    message.qos = QOS0;
    message.retained = 0;

    int rc;
    while ((rc = MQTTPublish(client, MQTT_TOPIC, &message)) != 0) {
        printf("Publish error, retrying: %d\n", rc);
    }
    printf("Published state: %s\n", state);
}

void setup_mqtt(Network* network, MQTTClient* client) {
    int rc;
    while (1) {
        rc = NetworkConnect(network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying: %d\n", rc);
            continue;
        }
        break;
    }

    MQTTClientInit(client, network, 1000, mqtt_send_buf, sizeof(mqtt_send_buf), mqtt_read_buf, sizeof(mqtt_read_buf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = NODE_NAME;

    while ((rc = MQTTConnect(client, &connectData)) != 0) {
        printf("MQTT connection failed, retrying: %d\n", rc);
    }
    printf("MQTT connected successfully\n");
}

int main(void) {
    Network network;
    MQTTClient client;

    setup_mqtt(&network, &client);

    while (1) {
        publish_state(&client, "work");
        publish_state(&client, "not work");
    }

    return 0;
}
