
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/sock.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "riot_esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 10000L

static volatile int messageArrived = 0;

void messageArrived(MessageData* data) {
    messageArrived = 1;
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    // Initialize the network
    NetworkInit(&network);

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    // Set up the connection data
    connectData.clientID.cstring = CLIENT_ID;
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    // Connect to the MQTT broker
    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    // Set the message arrived callback
    client.messageArrived = messageArrived;

    // Publish the state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        }

        // Wait for 5 seconds before publishing again
        xtimer_sleep(5);
    }

    // Disconnect from the MQTT broker
    MQTTDisconnect(&client);

    return 0;
}
