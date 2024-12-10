#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "mutex.h"
#include "timex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "net/gcoap.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_system.h"
#include "mbox.h"
#include "sys/socket.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "time_units.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_1"

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer; 
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("Network connected\n");
        break;
    }

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying...\n");
            NetworkDisconnect(&network);
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("MQTT connected\n");
        break;
    }

    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.payload = (void*)"work";
    message.payloadlen = strlen("work");

    while (1) {        
        rc = MQTTPublish(&client, MQTT_PUBLISH_TOPIC, &message);
        if (rc != 0) {
            printf("Error occurred posting the message.\n");
        } else {
            printf("Message published!\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}