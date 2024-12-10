#include "net/asymcute.h"
#include "mqtt.h"
#include "net/emcute.h"
#include "xtimer.h"
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"

static MQTTClient client;
static Network network;
static int interval_s = 5;

void publish_state(void) {
    MQTTMessage message;
    char *state = "work or not work";
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    message.qos = 1;
    message.retained = 0;
    MQTTPublish(&client, MQTT_TOPIC, &message);
    printf("Published state: %s\n", state);
}

int main(void) {
    unsigned char sendbuf[100], readbuf[100];

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    
    while (MQTTIsConnected(&client)) {
        publish_state();
        xtimer_sleep(interval_s);
    }
    
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    
    return 0;
}