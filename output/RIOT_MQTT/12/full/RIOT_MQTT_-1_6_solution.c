#include <stdio.h>
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "net/af.h"
#include "net/mqttsn.h"
#include "event/thread.h"
#include "ztimer.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "paho_mqtt.h"
#include "mutex.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "mbox.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "msg.h"
#include "unistd.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "timex.h"
#include "architecture.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

static const char* topic = "device/state";
static const char* message_work = "work";
static const char* message_not_work = "not work";

void mqtt_publish_state(MQTTClient* client, const char* msg) {
    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.payload = (void*)msg;
    message.payloadlen = strlen(msg);

    int rc = MQTTPublish(client, topic, &message);
    if (rc != 0) {
        printf("Error publishing: %d\n", rc);
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void) {
    unsigned char sendbuf[100], readbuf[100];
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    printf("Initializing network...\n");
    NetworkInit(&network);

    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connect failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connect successful\n");
            break;
        }
    }

    printf("Initializing MQTT client...\n");
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    options.MQTTVersion = 3;
    options.clientID.cstring = "esp32_client";

    while (1) {
        int rc = MQTTConnect(&client, &options);
        if (rc != 0) {
            printf("MQTT connect failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("MQTT connect successful\n");
            break;
        }
    }

    while (1) {
        const char* msg = (rand() % 2) == 0 ? message_work : message_not_work;
        mqtt_publish_state(&client, msg);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    return 0;
}