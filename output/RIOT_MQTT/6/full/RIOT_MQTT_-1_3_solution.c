#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "paho_mqtt.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/gcoap.h"
#include "net/protnum.h"
#include "net/emcute.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

void connect_to_broker(MQTTClient* client, Network* network) {
    int rc;
    while (1) {
        rc = NetworkConnect(network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying in 1 second...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = (char*)"ESP32_Client";

    while (1) {
        rc = MQTTConnect(client, &connectData);
        if (rc != 0) {
            printf("MQTT connect failed. Retrying in 1 second...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        } else {
            printf("MQTT connected successfully.\n");
            break;
        }
    }
}

void publish_state(MQTTClient* client) {
    MQTTMessage message;
    const char* topic = "esp32/state";
    char payload[] = "work or not work";

    message.qos = 1;
    message.retained = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    while (1) {
        int rc = MQTTPublish(client, topic, &message);
        if (rc != 0) {
            printf("Publish failed. Retrying in %d seconds...\n", PUBLISH_INTERVAL);
        } else {
            printf("State published successfully: %s\n", payload);
        }
        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connect_to_broker(&client, &network);
    publish_state(&client);

    return 0;
}