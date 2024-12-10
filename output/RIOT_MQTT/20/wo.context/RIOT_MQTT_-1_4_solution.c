#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define MQTT_BROKER_ADDRESS  "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define MQTT_PUBLISH_TOPIC   "device/state"
#define PUBLISH_INTERVAL_SEC 5

static int publish_state(MQTTClient *client, const char *state);
static void *main_loop(void *arg);

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    printf("[MQTT] Initializing network...\n");
    NetworkInit(&network);

    printf("[MQTT] Connecting to network...\n");
    if (NetworkConnect(&network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT) != SUCCESS) {
        printf("[MQTT] Failed to connect to network.\n");
        return -1;
    }

    printf("[MQTT] Initializing client...\n");
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "espmqttclient";

    printf("[MQTT] Connecting to MQTT broker...\n");
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("[MQTT] Failed to connect to MQTT broker.\n");
        return -1;
    }

    printf("[MQTT] Connected to MQTT broker.\n");

    /* Start main loop for publishing state */
    main_loop(&client);

    return 0;
}

static int publish_state(MQTTClient *client, const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    printf("[MQTT] Publishing state: %s\n", state);
    return MQTTPublish(client, MQTT_PUBLISH_TOPIC, &message);
}

static void *main_loop(void *arg) {
    MQTTClient *client = (MQTTClient *)arg;
    char *state = "work";

    while (1) {
        publish_state(client, state);
        usleep(PUBLISH_INTERVAL_SEC * 1000000);

        // Toggle state for simulation
        state = (strcmp(state, "work") == 0) ? "not work" : "work";
    }
    return NULL;
}