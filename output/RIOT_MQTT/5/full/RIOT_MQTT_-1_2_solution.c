#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "esp_wifi.h"
#include "paho_mqtt.h"
#include "thread.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL_SEC 5
#define MQTT_MSG "work or not work"

static char stack[THREAD_STACKSIZE_DEFAULT];

void* publisher_thread(void *arg) {
    (void)arg;
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    // Initialize network and MQTT client
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTClientInit(&client, &network, 1000, 0, 0, 0, 0);

    // MQTT connection
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_Client";
    MQTTConnect(&client, &connectData);

    while (1) {
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)MQTT_MSG;
        message.payloadlen = strlen(MQTT_MSG);

        MQTTPublish(&client, "riotsample/state", &message);

        // Wait for next publish
        sleep(PUBLISH_INTERVAL_SEC);
    }

    return NULL;
}

int main(void) {
    puts("MQTT Publish state every 5 seconds");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publisher_thread, NULL, "publish");
    return 0;
}
