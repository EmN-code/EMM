
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <ztimer.h>
#include <net/sock/udp.h>

#include "MQTTClient.h"  // Ensure this is the last header file to be included

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work or not work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT_MS 30000
#define SEND_BUF_SIZE 100
#define READ_BUF_SIZE 100

static Network network;
static MQTTClient client;
static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char readbuf[READ_BUF_SIZE];

void connect_to_broker(void) {
    int rc;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    rc = NetworkConnect(&network, EMQX_SERVER_IP, EMQX_SERVER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return;
    }

    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT_MS, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    int rc = MQTTPublish(&client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
