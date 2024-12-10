#include <ztimer.h>
#include <stdlib.h>
#include <event/thread.h>
#include <xtimer.h>
#include <stdoi.h>
#include <stddef.h>
#include <unistd.h>
#include <net/sock/util.h>  // Include for network utilities
#include <netinet/in.h>     // Include for network interactions
#include <arpa/inet.h>      // Include for address manipulation
#include <MQTTClient.h>     // Include for MQTTClient functions

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5   // 5 seconds
#define MQTT_MESSAGE "work"

int main(void) {
    Network network;
    MQTTClient client;
    int rc;
    unsigned char sendbuf[80], readbuf[80];
    MQTTMessage message;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying...\n");
            NetworkDisconnect(&network);
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        } else {
            printf("MQTT connected successfully.\n");
            break;
        }
    }

    while (1) {
        message.qos = 0;
        message.retained = 0;
        message.payload = (void *)MQTT_MESSAGE;
        message.payloadlen = strlen(MQTT_MESSAGE);

        rc = MQTTPublish(&client, "state", &message);
        if (rc != 0) {
            printf("MQTT publish failed.\n");
            NetworkDisconnect(&network);
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        } else {
            printf("Message published successfully: %s\n", MQTT_MESSAGE);
        }

        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL);
    }

    return 0;
}
