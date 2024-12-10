#include <string.h>
#include <stdlib.h>
#include "MQTTClient.h"  // Ensure this is the last header as per requirement

#include "xtimer.h"  // For timing purposes
#include "msg.h"     // For message passing between threads
#include "periph/gpio.h"  // For GPIO operations if needed
#include "net/netif.h"  // For network interface configurations

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID        "esp32_client"
#define TOPIC            "device/state"

static MQTTClient client;
static Network network;
static unsigned char sendbuf[128], readbuf[128];

void mqtt_publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    
    MQTTPublish(&client, TOPIC, &message);
}

void *mqtt_thread(void *arg) {
    (void)arg;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    while (1) {
        if (MQTTConnect(&client, &connectData) != SUCCESS) {
            printf("MQTT connection failed!");
            continue;
        }

        mqtt_publish_state("work");
        xtimer_sleep(5);

        mqtt_publish_state("not work");
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    netif_t *netif = netif_get_by_name("wlan0");
    if (!netif) {
        printf("Failed to get network interface\n");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    NetworkInit(&network, netif);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt");

    return 0;
}