#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <thread.h>
#include <unistd.h>
#include <timex.h>
#include <ztimer.h>
#include <event/thread.h>
#include <msg.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "MQTTClient.h"

#define MQTT_ADDRESS   "tcp://47.102.103.1:1883"
#define CLIENTID       "ESP32Client"
#define TOPIC          "state"
#define PAYLOAD        "work"
#define QOS            1
#define TIMEOUT        10000L

static char stack[THREAD_STACKSIZE_DEFAULT];

void *publish_thread(void *arg) {
    (void)arg;
    MQTTClient client;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, MQTT_ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return NULL;
    }

    while (1) {
        pubmsg.payload = PAYLOAD;
        pubmsg.payloadlen = (int)strlen(PAYLOAD);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered\n", token);
        
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish_thread");
    return 0;
}