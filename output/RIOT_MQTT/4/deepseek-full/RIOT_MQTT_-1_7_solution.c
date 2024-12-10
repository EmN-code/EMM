
#include <stddef.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdio.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <mutex.h>
#include <thread.h>
#include <stdlib.h>
#include <net/sock.h>
#include <net/emcute.h>
#include <ztimer.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000

static Network network;
static MQTTClient client;

void network_init(void) {
    NetworkInit(&network);
}

int connect_to_broker(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    return MQTTConnect(&client, &options);
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    network_init();

    unsigned char sendbuf[256];
    unsigned char readbuf[256];
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (connect_to_broker() != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
