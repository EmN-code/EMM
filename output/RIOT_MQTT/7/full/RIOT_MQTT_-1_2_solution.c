#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define PUBLISH_INTERVAL  (5U * US_PER_SEC)  // 5 seconds in microseconds

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    char *topic = "device/state";
    MQTTMessage message;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    // Initialize the network
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Establish network connection with retry
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            ztimer_sleep(ZTIMER_USEC, 1 * US_PER_SEC);
            continue;
        }
        printf("Network connected successfully.\n");
        break;
    }

    // Establish MQTT connection with retry
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != SUCCESS) {
            printf("MQTT connection failed. Retrying...\n");
            ztimer_sleep(ZTIMER_USEC, 1 * US_PER_SEC);
            continue;
        }
        printf("MQTT connected successfully.\n");
        break;
    }

    // Enter the publish loop
    while (1) {
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)"work";
        message.payloadlen = (strlen("work")+1);

        rc = MQTTPublish(&client, topic, &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message.\n");
        } else {
            printf("Message published successfully.\n");
        }

        // Sleep for 5 Seconds
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return 0;
}