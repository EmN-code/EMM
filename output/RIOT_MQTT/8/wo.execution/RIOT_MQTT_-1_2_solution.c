#include "net/gcoap.h"
#include "net/emcute.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "device/status"
#define MQTT_WORK_STATE "work"
#define MQTT_NOT_WORK_STATE "not work"
#define PUBLISH_INTERVAL (5U)

static MQTTClient client;

void publish_state(bool work_state) {
    const char *state = work_state ? MQTT_WORK_STATE : MQTT_NOT_WORK_STATE;
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    MQTTPublish(&client, MQTT_TOPIC, &message);
}

int main(void) {
    Network network;
    MQTTClientInit(&client, &network, 1000, malloc(MQTT_SENDBUF_SIZE), MQTT_SENDBUF_SIZE,
                    malloc(MQTT_READBUF_SIZE), MQTT_READBUF_SIZE);
    
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT Broker\n");
        return -1;
    }

    while (1) {
        publish_state(true);  // or replace true with your actual work state condition
        sleep(PUBLISH_INTERVAL);
    }

    return 0;
}