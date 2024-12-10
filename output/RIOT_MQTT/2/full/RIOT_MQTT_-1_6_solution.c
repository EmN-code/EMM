#include <net/sock/util.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <ztimer.h>
#include <timex.h>
#include <stdio.h>
#include <assert.h>
#include <mutex.h>
#include <dht_params.h>
#include <xtimer.h>
#include <time_units.h>
#include <dht.h>
#include <msg.h>
#include <thread.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <MQTTClient.h>  // Include the MQTT client at the end as per the requirement

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC      "state"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds
#define MQTT_MESSAGE     "work"

static char mqtt_sendbuf[1000];
static char mqtt_readbuf[1000];

static MQTTClient client;
static Network network;

static ztimer_t timer;

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
        printf("MQTTPublish failed\n");
    }
    else {
        printf("Published message: '%s'\n", MQTT_MESSAGE);
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 10000, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Network connect failed\n");
        return -1;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOTClient";

    if (MQTTConnect(&client, &options) != 0) {
        printf("MQTT connect failed\n");
        return -1;
    }

    ztimer_init(ZTIMER_MSEC);

    while (1) {
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
        publish_state();
    }
    return 0;
}
