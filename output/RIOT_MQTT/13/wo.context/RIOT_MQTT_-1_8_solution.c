#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/sock/tcp.h>
#include <MQTTClient.h>

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define STATE_TOPIC "device/state"
#define CLIENT_ID "esp32-client"

static MQTTClient client;
static Network network;

void publish_state(MQTTClient* client) {
    const char* message = "work";  // or "not work" based on actual state
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void*)message;
    mqtt_message.payloadlen = strlen(message);
    MQTTPublish(client, STATE_TOPIC, &mqtt_message);
    printf("Published: %s\n", message);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    if (NetworkConnect(&network, MQTT_SERVER_IP, MQTT_SERVER_PORT) != SUCCESS) {
        printf("Network connection failed\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;    
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connection failed\n");
        return -1;
    }

    while (1) {
        publish_state(&client);
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}