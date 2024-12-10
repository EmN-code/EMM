#include "xtimer.h"
#include "timex.h"
#include "net/sock/udp.h"
#include "net/mqttsn.h"
#include "net/mqtt.h"
#include "MQTTClient.h"

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC  "device/state"
#define MQTT_PAYLOAD_WORK     "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"
#define PUBLISH_INTERVAL      (5U * US_PER_SEC)

static MQTTClient client;
static Network network;

static void publish_state(MQTTClient* client) {
    MQTTMessage message;
    char* payload = MQTT_PAYLOAD_WORK;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    if (MQTTPublish(client, MQTT_TOPIC, &message) != SUCCESS) {
        printf("Error: Unable to publish\n");
    } else {
        printf("Published: %s\n", payload);
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 3000, NULL, 0, NULL, 0);
    
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != SUCCESS) {
        printf("Error: Unable to connect to broker\n");
        return -1;
    }
    
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Error: Unable to connect MQTT client\n");
        return -1;
    }

    while (1) {
        publish_state(&client);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}