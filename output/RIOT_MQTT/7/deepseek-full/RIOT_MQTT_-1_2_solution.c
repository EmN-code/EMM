
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/netopt.h>
#include <net/gnrc/netif.h>
#include <net/gnrc/netapi.h>
#include <net/gnrc/netreg.h>
#include <net/gnrc/ipv6.h>
#include <net/gnrc/udp.h>
#include <net/gnrc/tcp.h>
#include <net/gnrc/pktdump.h>
#include <net/gnrc/netif/hdr.h>
#include <net/gnrc/netif/default.h>
#include <net/gnrc/netif/ethernet.h>
#include <net/gnrc/netif/ieee802154.h>
#include <net/gnrc/netif/raw.h>
#include <net/gnrc/netif/init.h>
#include <net/gnrc/netif/internal.h>
#include <net/gnrc/netif/types.h>
#include <net/gnrc/netif/params.h>
#include <net/gnrc/netif/conf.h>

#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "riot_esp32"
#define TOPIC "state"
#define PAYLOAD "work"

static Network network;
static MQTTClient mqttClient;

static int mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = CLIENT_ID;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    int rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return -1;
    }
    printf("MQTT connected\n");
    return 0;
}

static int mqtt_publish(const char *topic, const char *payload) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(&mqttClient, topic, &message);
    if (rc != 0) {
        printf("MQTT publish failed\n");
        return -1;
    }
    printf("MQTT published\n");
    return 0;
}

int main(void) {
    printf("Starting MQTT example\n");

    // Initialize network
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;

    // Initialize MQTT client
    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    // Connect to MQTT broker
    if (mqtt_connect() != 0) {
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        if (mqtt_publish(TOPIC, PAYLOAD) != 0) {
            return -1;
        }
        xtimer_sleep(5);
    }

    return 0;
}
