#include "xtimer.h"          // Necessary for timer functionality
#include "net/sock.h"        // Network socket functions
#include "MQTTClient.h"       // MQTT client header must be last

char sendbuf[100];
char readbuf[100];

int main(void) {
    printf("Starting MQTT client...\n");

    // Prepare the network interface
    Network network;
    MQTTClient client;
    unsigned int timeout = 1000; // 1 second timeout

    // Initialize network with default function
    NetworkInit(&network);

    // Connect to the specified MQTT broker at IP 47.102.103.1, port 1883
    NetworkConnect(&network, "47.102.103.1", 1883);

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, timeout, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // MQTT connection data
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_Client";

    // Connect to MQTT broker
    if (MQTTConnect(&client, &data) != SUCCESS) {
        printf("ERROR: Unable to connect to MQTT broker\n");
        return -1;
    } else {
        printf("Connected to MQTT broker\n");
    }

    // Loop to publish messages every 5 seconds
    while (1) {
        char* payload = "work";
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = payload;
        message.payloadlen = strlen(payload);

        // Publish the msg to the topic "state"
        MQTTPublish(&client, "state", &message);

        printf("Published: %s\n", payload);

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}