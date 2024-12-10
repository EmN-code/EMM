
#include <net/sock.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <thread.h>
#include <stdatomic.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <dht.h>
#include <ztimer.h>
#include <stddef.h>
#include <net/sock/udp.h>

#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 10000L

static volatile bool is_connected = false;

static void mqtt_connection_lost(void *context, char *cause)
{
    (void)context;
    (void)cause;
    is_connected = false;
    printf("Connection lost\n");
}

static int mqtt_message_arrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    (void)context;
    (void)topicName;
    (void)topicLen;
    (void)message;
    return 1;
}

static void mqtt_delivery_complete(void *context, MQTTClient_deliveryToken dt)
{
    (void)context;
    (void)dt;
}

static void connect_to_broker(MQTTClient *client, const char *server_ip, int server_port)
{
    Network network;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    NetworkInit(&network);
    MQTTClient_create(client, &network, 1000);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(*client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return;
    }

    is_connected = true;
    printf("Connected to MQTT broker\n");
}

static void publish_state(MQTTClient *client)
{
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    pubmsg.payload = (void *)PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_publishMessage(*client, TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(*client, token, TIMEOUT);
    printf("Message '%s' with delivery token %d delivered\n", PAYLOAD, token);
}

int main(void)
{
    MQTTClient client;
    MQTTClient_setCallbacks(&client, NULL, mqtt_connection_lost, mqtt_message_arrived, mqtt_delivery_complete);

    connect_to_broker(&client, SERVER_IP, SERVER_PORT);

    while (1) {
        if (is_connected) {
            publish_state(&client);
        }
        xtimer_sleep(5);
    }

    MQTTClient_disconnect(&client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}
