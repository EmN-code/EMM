
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <msg.h>
#include <time_units.h>
#include <ztimer.h>
#include <timex.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>  // Ensure this is the last header file to be included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_TIMEOUT 10000L

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void publish_state(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&client, MQTT_TOPIC, &message);
}

int main(void) {
    // Initialize the network
    NetworkInit(&network);

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, MQTT_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
