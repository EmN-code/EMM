#include <arpa/inet.h>
#include <event/thread.h>
#include <ztimer.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <paho_mqtt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <mutex.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <stdio.h>
#include <mbox.h>
#include <stdatomic.h>
#include <esp_system.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <kernel_defines.h>
#include <net/emcute.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <stddef.h>
#include <msg.h>
#include <esp_wifi.h>
#include <net/gcoap.h>
#include <timex.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD_ON "work"
#define MQTT_PAYLOAD_OFF "not work"
#define SLEEP_INTERVAL 5 

static int connect_to_network(Network *network, const char *address, int port) {
    int rc;
    while (1) {
        rc = NetworkConnect(network, (char *)address, port);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }
    return rc;
}

static int connect_to_mqtt_broker(MQTTClient *client, Network *network) {
    int rc;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;

    while (1) {
        rc = MQTTConnect(client, &options);
        if (rc != 0) {
            printf("MQTT connection failed, retrying...\n");
            xtimer_sleep(1);
        } else {
            printf("MQTT connected successfully\n");
            break;
        }
    }

    return rc;
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];
    Network network;
    MQTTClient client;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connect_to_network(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    connect_to_mqtt_broker(&client, &network);

    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.dup = 0;

    while (1) {
        message.payload = (void *)MQTT_PAYLOAD_ON;
        message.payloadlen = strlen(MQTT_PAYLOAD_ON);
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish, retrying...\n");
            connect_to_mqtt_broker(&client, &network);
            continue;
        } else {
            printf("Published: %s\n", MQTT_PAYLOAD_ON);
        }

        xtimer_sleep(SLEEP_INTERVAL);

        message.payload = (void *)MQTT_PAYLOAD_OFF;
        message.payloadlen = strlen(MQTT_PAYLOAD_OFF);
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish, retrying...\n");
            connect_to_mqtt_broker(&client, &network);
        } else {
            printf("Published: %s\n", MQTT_PAYLOAD_OFF);
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}