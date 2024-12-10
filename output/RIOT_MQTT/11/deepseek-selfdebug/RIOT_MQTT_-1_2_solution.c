
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_STACK_SIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static void *mqtt_thread(void *arg)
{
    (void)arg;

    struct Network network;
    struct MQTTClient client;
    unsigned char sendbuf[256], readbuf[256];

    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = MQTT_MESSAGE;
    pubmsg.payloadlen = strlen(MQTT_MESSAGE);
    pubmsg.qos = 0;
    pubmsg.retained = 0;

    MQTTClient_deliveryToken token;

    while (1) {
        if (MQTTClient_create(&client, &network, "RIOT_MQTT_CLIENT", MQTTCLIENT_PERSISTENCE_NONE, NULL) != MQTTCLIENT_SUCCESS) {
            printf("Failed to create client\n");
            return NULL;
        }

        if (MQTTClient_setCallbacks(&client, NULL, NULL, NULL, NULL) != MQTTCLIENT_SUCCESS) {
            printf("Failed to set callbacks\n");
            MQTTClient_destroy(&client);
            return NULL;
        }

        if (MQTTClient_connect(&client, &conn_opts) != MQTTCLIENT_SUCCESS) {
            printf("Failed to connect\n");
            MQTTClient_destroy(&client);
            return NULL;
        }

        printf("Connected to MQTT broker\n");

        if (MQTTClient_publishMessage(&client, MQTT_TOPIC, &pubmsg, &token) != MQTTCLIENT_SUCCESS) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published\n");
        }

        MQTTClient_disconnect(&client, 10000);
        MQTTClient_destroy(&client);

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    thread_create(NULL, MQTT_STACK_SIZE, MQTT_THREAD_PRIORITY, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
