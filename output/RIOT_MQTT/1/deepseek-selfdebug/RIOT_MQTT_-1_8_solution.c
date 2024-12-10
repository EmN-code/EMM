
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static void mqtt_publish(void) {
    static uint8_t buf[128];
    static struct Network network;
    static MQTTClient client;

    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    int rc;

    MQTTClient_create(&client, MQTT_BROKER_IP, MQTT_CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = MQTT_KEEPALIVE;
    conn_opts.cleansession = 1;
    conn_opts.username = MQTT_USERNAME;
    conn_opts.password = MQTT_PASSWORD;

    if ((rc = MQTTClient_connect(&client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return;
    }

    pubmsg.payload = (void*)MQTT_MESSAGE;
    pubmsg.payloadlen = strlen(MQTT_MESSAGE);
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = MQTT_RETAIN;

    MQTTClient_publishMessage(&client, MQTT_TOPIC, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
           "on topic %s for client with ClientID: %s\n",
           (int)(MQTT_KEEPALIVE/1000), MQTT_MESSAGE, MQTT_TOPIC, MQTT_CLIENT_ID);

    rc = MQTTClient_waitForCompletion(&client, token, MQTT_KEEPALIVE);
    printf("Message with delivery token %d delivered\n", token);

    MQTTClient_disconnect(&client, 10000);
    MQTTClient_destroy(&client);
}

int main(void) {
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }
    return 0;
}
