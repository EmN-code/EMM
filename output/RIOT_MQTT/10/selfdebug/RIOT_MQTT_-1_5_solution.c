#include <xtimer.h>
#include <stdio.h>
#include <net/mqtt_client.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "device/state"
#define MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

/* structure to hold the network configuration */
static sock_udp_ep_t broker_endpoint = {
    .family = AF_INET,
    .port = BROKER_PORT
    // Set the IP later after conversion
};

int main(void) {
    /* Resolving broker address */
    if (inet_pton(AF_INET, BROKER_IP, &broker_endpoint.addr.ipv4) != 1) {
        printf("Error: unable to parse broker IP\n");
        return -1;
    }

    /* Initialize MQTT client */
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    /* Establishing connection to the broker */
    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Error: unable to connect to broker\n");
        return -1;
    }

    while (1) {
        /* Prepare and publish message */
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)MESSAGE;
        message.payloadlen = strlen(MESSAGE);

        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Error: unable to publish message\n");
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}