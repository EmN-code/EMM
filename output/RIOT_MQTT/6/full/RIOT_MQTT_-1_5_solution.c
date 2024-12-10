#include <ztimer.h>
#include <msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xtimer.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mutex.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <arch/xtimer.h> // Assuming used for timer
#include <time_units.h> // Assuming used for handling units
#include <esp_system.h>
#include <esp_wifi.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <architecture.h>
#include <stdatomic.h>
#include <paho_mqtt.h>
#include <net/gcoap.h>
#include <net/protnum.h>
#include <architecture.h>
#include <thread.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID    "esp32_client"
#define TOPIC    "state/status"

void mqtt_task(void)
{
    int rc;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    // Initialize network and client
    NetworkInit(&network);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            // Network connection failed, retry
            printf("Network connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            // MQTT connection failed, retry
            printf("MQTT connection failed, retrying...\n");
            NetworkDisconnect(&network);
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        } else {
            printf("MQTT connected successfully\n");
            break;
        }
    }

    // Prepare message
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)"work or not work";
    message.payloadlen = strlen("work or not work");

    while (1) {
        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            // Publish failed
            printf("Publish failed, retrying...\n");
            MQTTConnect(&client, &connectData);  // Reconnect if needed
        } else {
            printf("Message published: 'work or not work'\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
}

int main(void)
{
    mqtt_task();
    return 0;
}
