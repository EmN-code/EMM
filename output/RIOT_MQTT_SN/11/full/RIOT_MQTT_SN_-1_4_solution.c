#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         1883
#define MQTT_TOPIC          "device/state"
#define SEND_INTERVAL_SEC   (5U)
#define MQTT_BUFFER_SIZE    128

static char mqtt_stack[THREAD_STACKSIZE_MAIN];
static MQTTClient client;
static Network network;
static unsigned char sendbuf[MQTT_BUFFER_SIZE], readbuf[MQTT_BUFFER_SIZE];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)"work or not work";
    message.payloadlen = strlen("work or not work");

    int rc;
    while (1) {
        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Error publishing message: %d\n", rc);
            // Re-connect if needed
            rc = MQTTConnect(&client, (MQTTPacket_connectData*)&MQTTPacket_connectData_initializer);
            if (rc != 0) {
                printf("Error reconnecting to MQTT broker\n");
                ztimer_sleep(ZTIMER_MSEC, 1000);
                continue;
            }
        }
        break;
    }
    printf("Message published successfully\n");
}

int main(void) {
    MQTTClientInit(&client, &network, 1000, sendbuf, MQTT_BUFFER_SIZE, readbuf, MQTT_BUFFER_SIZE);
    int rc;
    /* Connect to network */
    while (1) {
        rc = NetworkConnect(&network, SERVER_ADDR, SERVER_PORT);
        if (rc != 0) {
            printf("Network connection failed\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        break;
    }
    printf("Connected to network\n");

    /* Connect to the MQTT broker */
    rc = MQTTConnect(&client, (MQTTPacket_connectData*)&MQTTPacket_connectData_initializer);
    while (rc != 0) {
        printf("MQTT connection failed. Retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
        rc = MQTTConnect(&client, (MQTTPacket_connectData*)&MQTTPacket_connectData_initializer);
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_SEC, SEND_INTERVAL_SEC);
    }

    return 0;
}