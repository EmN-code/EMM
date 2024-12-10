
#include <stddef.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <thread.h>
#include <stdlib.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <ztimer.h>
#include <msg.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLEANSESSION 1

static Network network;
static MQTTClient client;
static MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

void mqtt_connect(void) {
    connectData.keepAliveInterval = MQTT_KEEPALIVE;
    connectData.cleansession = MQTT_CLEANSESSION;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &connectData) != 0) {
        puts("MQTT connection failed");
        return;
    }
    puts("MQTT connected");
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
        puts("MQTT publish failed");
        return;
    }
    puts("MQTT message published");
}

void mqtt_loop(void) {
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    if (network.connect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        puts("Network connection failed");
        return 1;
    }
    puts("Network connected");

    mqtt_connect();
    mqtt_loop();

    return 0;
}
