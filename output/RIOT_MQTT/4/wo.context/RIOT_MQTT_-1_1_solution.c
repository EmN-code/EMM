#include "mqtt_client.h"
#include "net/emcute.h"
#include "embunit"  // Assuming necessary for additional unit testing or infrastructure
#include <stdio.h>

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_PORT        1883
#define PUBLISH_INTERVAL_SEC 5
#define BUFFER_SIZE      128

static char sendbuf[BUFFER_SIZE];
static char recvbuf[BUFFER_SIZE];
static MQTTClient client;
static Network network;

void publish_state(void) {
    static int state = 0;
    const char* msg = state % 2 == 0 ? "work" : "not work";
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)msg;
    message.payloadlen = strlen(msg);
    MQTTPublish(&client, "state", &message);
    state++;
}

int main(void) {
    // Initialize Network and MQTT Client
    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, recvbuf, BUFFER_SIZE);

    // This is platform-specific. On ESP32, additional setup would be necessary
    // Connect to the network (WiFi, Ethernet, etc.)
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_PORT) != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    // Connect to MQTT broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "unique_client_id";
    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Publishing the state every 5 seconds
    while (1) {
        publish_state();
        sleep(PUBLISH_INTERVAL_SEC); // Assuming POSIX sleep for simplicity
    }

    return 0;
}