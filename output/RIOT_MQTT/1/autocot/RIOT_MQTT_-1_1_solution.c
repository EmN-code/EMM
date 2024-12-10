#include <ztimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>
#include <string.h>

// Network and MQTT client instances
Network network;
MQTTClient client;

// MQTT server configuration
char* address_ip = "47.102.103.1";
int port_number = 1883;
char* topic = "device/state";

// Publish interval and payload
#define PUBLISH_INTERVAL 5
char* state_payload = "work";

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)state_payload;
    message.payloadlen = strlen(state_payload);

    if (MQTTPublish(&client, topic, &message) != SUCCESS) {
        printf("Error: Unable to publish message\n");
    }
}

int main(void) {
    // Initialize network
    NetworkInit(&network);
    if (NetworkConnect(&network, address_ip, port_number) != SUCCESS) {
        printf("Error: Unable to connect to network\n");
        return -1;
    }

    // Initialize MQTT client
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "ESP32Client";

    // Connect to MQTT broker
    if (MQTTConnect(&client, &data) != SUCCESS) {
        printf("Error: Unable to connect to MQTT broker\n");
        return -1;
    }

    // Main loop
    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * 1000);
    }

    return 0;
}