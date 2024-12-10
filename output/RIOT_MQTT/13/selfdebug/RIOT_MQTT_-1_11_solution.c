#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "net/sock/udp.h"
#include "net/sock/mqtt.h"

void publish_message(MQTTClient *client, const char *message) {
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = false;
    mqtt_message.payload = (void *) message;
    mqtt_message.payloadlen = strlen(message);
    
    MQTTPublish(client, "state_topic", &mqtt_message);
}

void connect_to_broker(MQTTClient *client, Network *network) {
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "esp32_client";

    if (MQTTConnect(client, &connect_data) != SUCCESS) {
        printf("Failed to connect to broker\n");
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    network.mqttread = NULL;  // Define appropriately
    network.mqttwrite = NULL;  // Define appropriately

    NetworkConnect(&network, "47.102.103.1", 1883);
    connect_to_broker(&client, &network);

    while (1) {
        publish_message(&client, "work");
        sleep(5);
    }

    return 0;
}