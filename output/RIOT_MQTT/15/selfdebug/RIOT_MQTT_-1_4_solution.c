#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "net/mqttsn.h"
#include "net/sock_udp.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static mqttsn_t mqttsn;
static sock_udp_ep_t broker;

void on_pub_comp(void *arg, int status)
{
    (void)arg;
    if (status == MQTT_OK) {
        printf("Publish successful\n");
    }
    else {
        printf("Publish failed with status: %d\n", status);
    }
}

int main(void)
{
    char message[] = "work";
    broker.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr);
    broker.netif = SOCK_UNSPEC;
    broker.port = MQTT_BROKER_PORT;

    mqttsn_pkt_t pkt;

    mqttsn_client_init(&mqttsn, &broker, &pkt);

    while (1) {
        mqttsn_pub(&mqttsn, message, strlen(message), on_pub_comp, NULL);

        if (strcmp(message, "work") == 0) {
            strcpy(message, "not work");
        } else {
            strcpy(message, "work");
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}