#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <event/thread.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"

// Mock function representing the machine's state
const char* get_machine_state() {
    // In a full implementation, machine state is determined by actual data
    return MQTT_MESSAGE_WORK;
}

int main() {
    // Buffers for the MQTT client
    unsigned char sendbuf[100];
    unsigned char readbuf[100];

    Network network;
    MQTTClient client;
    int rc = 0;

    // Initialize network and MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker with retry logic
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to network successfully.\n");
            break;
        }
    }

    // Prepare the MQTT message
    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;

    // Publish messages indefinitely
    while (1) {
        message.payload = (void *)get_machine_state();
        message.payloadlen = strlen((const char *)message.payload);

        // Attempt to publish and check the result
        if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
            printf("Failed to publish message: %d\n", rc);
        } else {
            printf("Message published successfully\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}