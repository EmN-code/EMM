#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "paho_mqtt.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32Client"

static ztimer_t timer;

void publish_state(MQTTClient* client) {
    const char* topic = "/device/state";
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)"work";
    message.payloadlen = strlen("work");
    MQTTPublish(client, topic, &message);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        return -1;
    }
    
    ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds before starting loop
    
    while (1) {
        publish_state(&client);
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }

    return 0;
}