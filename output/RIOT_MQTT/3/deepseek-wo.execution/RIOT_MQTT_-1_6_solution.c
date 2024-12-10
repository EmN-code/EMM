
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

#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "riot_esp32"
#define MQTT_KEEP_ALIVE 20
#define MQTT_CLEAN_SESSION true
#define MQTT_QOS 1
#define MQTT_RETAIN false

static volatile bool mqtt_connected = false;

void mqtt_connection_lost(void *context, char *cause)
{
    (void)context;
    (void)cause;
    mqtt_connected = false;
    printf("MQTT connection lost\n");
}

int mqtt_message_arrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    (void)context;
    (void)topicName;
    (void)topicLen;
    (void)message;
    return 1;
}

void mqtt_delivery_complete(void *context, MQTTClient_deliveryToken dt)
{
    (void)context;
    (void)dt;
}

int main(void)
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, MQTT_BROKER_IP, MQTT_CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = MQTT_KEEP_ALIVE;
    conn_opts.cleansession = MQTT_CLEAN_SESSION;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return -1;
    }

    mqtt_connected = true;

    MQTTClient_setCallbacks(client, NULL, mqtt_connection_lost, mqtt_message_arrived, mqtt_delivery_complete);

    while (1) {
        if (mqtt_connected) {
            MQTTClient_message pubmsg = MQTTClient_message_initializer;
            MQTTClient_deliveryToken token;
            char payload[] = "work";

            pubmsg.payload = payload;
            pubmsg.payloadlen = strlen(payload);
            pubmsg.qos = MQTT_QOS;
            pubmsg.retained = MQTT_RETAIN;

            MQTTClient_publishMessage(client, MQTT_TOPIC, &pubmsg, &token);
            MQTTClient_waitForCompletion(client, token, 10000L);

            printf("Message '%s' delivered\n", payload);
        }

        xtimer_sleep(5);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}
