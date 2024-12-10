#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <stdint.h>
#include <stdatomic.h>
#include <stddef.h>
#include <MQTTClient.h>

// MQTT configuration
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "state"

void *publish_state(void *arg) {
    MQTTClient client;
    Network network;
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    MQTTConnect(&client, &connectData);

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen("work");

        MQTTPublish(&client, MQTT_TOPIC, &message);

        struct timespec req;
        req.tv_sec = 5;
        req.tv_nsec = 0;
        nanosleep(&req, NULL);
    }

    return NULL;
}

int main(void) {
    publish_state(NULL);
    return 0;
}