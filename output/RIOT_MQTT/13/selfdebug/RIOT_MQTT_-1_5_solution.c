#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "device/state"

// Function to simulate work state
const char *get_state(void) {
    static int toggle = 0;
    toggle = !toggle;
    return toggle ? "work" : "not work";
}

void *mqtt_client_thread(void *arg) {
    (void)arg;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[128], readbuf[128];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    // Initialize network
    NetworkInit(&network);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to network\n");
        return NULL;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // MQTT connect options
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    // Connect to MQTT broker
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    // Prepare the MQTT message
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)get_state();
    message.payloadlen = strlen(message.payload);

    while (1) {
        // Publish state
        if (MQTTPublish(&client, PUB_TOPIC, &message) != SUCCESS) {
            printf("Failed to publish message\n");
        }
        sleep(5);

        // Update the state
        message.payload = (void*)get_state();
        message.payloadlen = strlen(message.payload);
    }
    return NULL;
}

int main(void) {
    // Create a thread for the MQTT client
    thread_create(mqtt_client_thread, NULL);
    return 0;
}