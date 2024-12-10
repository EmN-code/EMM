#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/mqtt.h"
#include "net/mqttsn.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "ztimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_TOPIC "device/state"
#define CLIENT_ID "esp32_device"

static mqtt_client_t mqtt_client;
static char mqtt_buffer[128];
static sock_udp_ep_t broker;

void on_puback(void *arg, int status) {
    if (status == MQTT_OK) {
        printf("Publish acknowledgment received\n");
    } else {
        printf("Failed to receive publish acknowledgment\n");
    }
}

int main(void) {
    broker.port = MQTT_BROKER_PORT;
    ipv6_addr_set_u8((ipv6_addr_t *)&broker.addr.ipv6, 47, 102, 103, 1);

    mqtt_client_init(&mqtt_client, &broker, mqtt_buffer, sizeof(mqtt_buffer));
    mqtt_connect(&mqtt_client, CLIENT_ID, NULL, NULL, 0, 0, 0);
    
    mqtt_pub_t pub;
    pub.topic = PUB_TOPIC;
    pub.msg = "work";
    pub.msg_len = strlen(pub.msg);
    pub.qos = 0;
    pub.retain = 0;

    while (1) {
        mqtt_publish(&mqtt_client, &pub, on_puback, NULL);
        ztimer_sleep(ZTIMER_USEC, 5000000);  // sleep 5 seconds
    }

    return 0;
}
