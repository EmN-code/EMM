#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "timex.h"
#include "thread.h"
#include "event/thread.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "architecture.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "device/state"
#define PUBLISH_INTERVAL 5

int main(void) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    int rc = 0;

    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32";

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying in 1 second...\n");
            sleep(1);
            continue;
        }
        printf("Network connected\n");

        while ((rc = MQTTConnect(&client, &connectData)) != 0) {
            printf("MQTT connection failed, return code %d. Retrying in 1 second...\n", rc);
            sleep(1);
        }
        printf("MQTT connected\n");

        MQTTMessage message;
        message.qos = 1;
        message.retained = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen((char*)message.payload);

        while (1) {
            rc = MQTTPublish(&client, MQTT_PUB_TOPIC, &message);
            if (rc != 0) {
                printf("Failed to publish, return code %d\n", rc);
            } else {
                printf("Message published: %s\n", (char*)message.payload);
            }

            sleep(PUBLISH_INTERVAL);

            // Alternate the payload
            if (strcmp((char*)message.payload, "work") == 0) {
                message.payload = (void*)"not work";
            } else {
                message.payload = (void*)"work";
            }
            message.payloadlen = strlen((char*)message.payload);
        }
    }

    return 0;
}