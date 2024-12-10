#include <stdio.h>
#include <string.h>
#include <time.h>

#include "xtimer.h"  // RIOT provides xtimer for delay purposes
#include "net/emcute.h"  // Not using this library, as requested
#include "net/sock/udp.h"  // For socket programming

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "esp32/state"
#define STATE_MESSAGE "work or not work"
#define INTERVAL (5 * US_PER_SEC)  // 5 seconds interval

static void publish_state(void) {
    MQTTClient client;
    Network network;
    int rc;
    unsigned char sendbuf[80], readbuf[80];
    MQTTMessage message;

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to MQTT broker.\n");
            break;
        }
    }

    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = MQTTConnect(&client, NULL)) != 0) {
        printf("MQTT connect failed, return code %d\n", rc);
    } else {
        printf("MQTT connected\n");
    }

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)STATE_MESSAGE;
    message.payloadlen = (int)strlen(STATE_MESSAGE);  

    while (1) {
        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message, return code %d. Retrying...\n", rc);
            continue;
        }

        printf("Message published: %s\n", STATE_MESSAGE);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
}

int main(void) {
    printf("MQTT Publisher is starting.\n");
    publish_state();
    return 0;
}