#include <ztimer.h>
#include <xtimer.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <msg.h>
#include <timex.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <event/thread.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL_SEC 5
#define TOPIC "esp32/status"
#define MQTT_DEVICE_ID "esp32_device"

enum { SERVER_MSG_QUEUE_SIZE = 8 };
static msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void publish_state(void) {
    MQTTMessage message;
    char payload[20];
    sprintf(payload, "work or not work");

    message.payload = payload;
    message.payloadlen = strlen(payload);
    message.qos = QOS0;
    message.retained = 0;

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
    }
}

int connect_mqtt(void) {
    int rc;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        break;
    }

    data.willFlag = 0;
    data.MQTTVersion = 4;
    data.clientID.cstring = MQTT_DEVICE_ID;
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    while (MQTTConnect(&client, &data) != 0) {
        printf("MQTT connection failed\n");
        NetworkDisconnect(&network);
        ztimer_sleep(ZTIMER_MSEC, 1000);
        NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    }
    printf("MQTT connected\n");
    return 0;
}

int main(void) {
    msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (connect_mqtt() != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL_SEC);
    }

    return 0;
}