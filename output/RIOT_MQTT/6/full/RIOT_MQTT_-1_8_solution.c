#include "architecture.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "thread.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "time_units.h"
#include "timex.h"
#include "ztimer.h"
#include "xtimer.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "mutex.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/protnum.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PUBLISH_INTERVAL 5

void publish_state(MQTTClient* client) {
    const char* states[] = {"work", "not work"};
    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.dup = 0;

    while (1) {
        for (int i = 0; i < 2; ++i) {
            message.payload = (void*)states[i];
            message.payloadlen = strlen(states[i]);
            int rc = MQTTPublish(client, MQTT_TOPIC, &message);
            if (rc != 0) {
                printf("Publish failed with return code %d, trying again...\n", rc);
                i--;
            } else {
                printf("Published state: %s\n", states[i]);
                ztimer_sleep(ZTIMER_SEC, MQTT_PUBLISH_INTERVAL);
            }
        }
    }
}

int main(void) {
    printf("Starting MQTT client example...\n");

    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }
        printf("Network connected\n");

        MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
        connectData.MQTTVersion = 3;
        connectData.clientID.cstring = "esp32Client";

        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed, retrying...\n");
            NetworkDisconnect(&network);
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }
        printf("MQTT connected\n");

        publish_state(&client);
        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }

    return 0;
}
