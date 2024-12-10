#include "xtimer.h"
#include "stdio.h"
#include "paho_mqtt.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"

static MQTTClient client;
static Network network;
static unsigned char sendbuf[1000], readbuf[1000];

void publish_status(void) {
    MQTTMessage message;
    const char *topic = "state/status";

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)"work";
    message.payloadlen = 4;

    if (MQTTPublish(&client, topic, &message) != SUCCESS) {
        printf("Error: Unable to publish message!\n");
    }
    else {
        printf("Published: %s\n", (char *)message.payload);
    }
}

int main(void) {
    int rc = 0;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    printf("Connecting to %s:%d\n", SERVER_IP, SERVER_PORT);
    if ((rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT)) != 0) {
        printf("Unable to connect to server: %d\n", rc);
        return rc;
    }

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("MQTT connect error: %d\n", rc);
        return rc;
    } else {
        printf("Successfully connected to MQTT broker\n");
    }

    while (1) {
        publish_status();
        xtimer_sleep(5);
    }

    return 0;
}
