#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define MQTT_TOPIC "esp32/status"
#define MESSAGE_PUB_INTERVAL 5    // seconds

static unsigned char sendbuf[128], readbuf[128];

int main(void) {
    int rc = 0;
    Network network;
    MQTTClient client;
    MQTTMessage message;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Network initialization can be customized depending on the actual mechanism used in RIOT
    // For the sake of demonstration, assumed initialization here.

    // Initialize network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Retry mechanism for network connection
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            sleep(1);
            continue;
        } else {
            printf("Network connected\n");
            break;
        }
    }

    // Prepare connection data
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    // Retry mechanism for MQTT connection
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying...\n");
            sleep(1);
            continue;
        } else {
            printf("MQTT connected\n");
            break;
        }
    }

    // Setup the message
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)"work or not work";
    message.payloadlen = strlen("work or not work");

    // Publish in a loop
    while (1) {
        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Message publish failed\n");
        } else {
            printf("Message published\n");
        }
        sleep(MESSAGE_PUB_INTERVAL);
    }

    // NetworkDisconnect(&network); // Uncomment if you plan to disconnect when leaving

    return 0;
}