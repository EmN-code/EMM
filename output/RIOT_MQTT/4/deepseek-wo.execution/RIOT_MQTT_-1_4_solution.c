
#include <net/sock.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <stdio.h>
#include <net/sock/tcp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <ztimer.h>
#include <stddef.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
