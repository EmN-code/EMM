#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "periph/gpio.h"
#include "thread.h"
#include "stdio.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "ESP32_MQTT_Client"
#define TOPIC "device/state"
#define INTERVAL (5 * 1000000U)  // publish interval in microseconds (5 seconds)

static MQTTClient client;
static Network network;

void publish_state(MQTTClient* client) {
    const char* message = "work"; // example payload, modify as needed
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void*)message;
    mqtt_message.payloadlen = strlen(message);
    MQTTPublish(client, TOPIC, &mqtt_message);
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != SUCCESS) {
        puts("Failed to connect to network");
        return 1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker.");

    while (1) {
        publish_state(&client);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}