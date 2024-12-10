#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "msg.h"
#include "mbox.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_TOPIC "device/state"
#define PUB_MSG "work or not work"
#define COMMAND_TIMEOUT_MS (4000)
#define RECONNECT_DELAY_SEC (1U)
#define PUBLISH_INTERVAL_SEC (5U)

static unsigned char sendbuf[100];
static unsigned char readbuf[100];

int main(void) {
    MQTTClient client;
    Network network;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    int rc = 0;

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

        if (rc != 0) {
            printf("Failed to connect to the network. Retrying in %d second(s).\n", RECONNECT_DELAY_SEC);
            ztimer_sleep(ZTIMER_SEC, RECONNECT_DELAY_SEC);
            continue;
        }
        else {
            printf("Connected to network successfully.\n");
            break;
        }
    }

    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_ESP32_Client";
    
    while (1) {
        rc = MQTTConnect(&client, &options);

        if (rc != 0) {
            printf("MQTT connection failed. Retrying in %d second(s).\n", RECONNECT_DELAY_SEC);
            ztimer_sleep(ZTIMER_SEC, RECONNECT_DELAY_SEC);
            continue;
        }
        else {
            printf("MQTT connected successfully.\n");
            break;
        }
    }

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)PUB_MSG;
    message.payloadlen = strlen(PUB_MSG);

    while (1) {
        rc = MQTTPublish(&client, PUB_TOPIC, &message);

        if (rc != 0) {
            printf("MQTT publish failed. Retrying in %d second(s).\n", RECONNECT_DELAY_SEC);
            ztimer_sleep(ZTIMER_SEC, RECONNECT_DELAY_SEC);
            continue;
        }
        else {
            printf("MQTT message published successfully: '%s'\n", PUB_MSG);
        }

        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL_SEC);
    }

    return 0;
}